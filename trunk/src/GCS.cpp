#include "GCS.h"
#include "Params.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include "../gui/MainWindow.h"

int GCS::counter = 1;

GCS::GCS(const Grammar& grammar, const QList<Sentence> list, MainWindow& parent) :
	QThread(), initGrammar(grammar), mSentences(list), parent(parent)
{
	threadNumber = counter++;
	bufferBusy = false;
}

void GCS::run()
{
	//	qDebug() << QString() + __FUNCTION__ + " start";
	Params& p = Params::instance();
	QDateTime t = QDateTime::currentDateTime();
	QDir dir;
	QString path = "output/" + t.toString("yyyy.MM.dd");
	if (!dir.exists(path))
	{
		dir.mkpath(path);
	}
	QFile file(path + "/" + t.toString("hh:mm:ss") + "_thread_" + QString::number(threadNumber) + ".csv");
	file.open(QFile::WriteOnly);
	QTextStream stream(&file);
	float sum = 0;
	float maxFitness = 0.0;
	int avgSteps = 0;
	int successIters = 0;
	int iters, maxSteps;
	if (p.learningMode)
	{
		iters = p.iterations;
		maxSteps = p.maxEvolutionSteps;
	}
	else
	{
		iters = 1;
		maxSteps = 1;
	}
	for (int iter = 0; iter < iters; iter++)
	{
		emit iterChanged(iter);
		tempGrammar = initGrammar;
		tempGrammar.clearFitness();
		stream << "\niteration_" << iter + 1;
		int step = 0;
		int nextEmit = 20;
		emit stepChanged(step);
//		stream << "," + QString::number(tempGrammar.fitness());
		while (step < maxSteps && (!p.endOnFullFitness || tempGrammar.fitness() < 1.0))
		{
			tempGrammar.induct(mSentences, *this);
			tempGrammar.computeFitness();
			stream << "\n" + QString::number(tempGrammar.fitness());
			if (tempGrammar.fitness() > maxFitness)
			{
				maxFitness = tempGrammar.fitness();
				bestGrammar = tempGrammar;
			}
			if (p.learningMode && p.allowGA && (tempGrammar.fitness() < 1.0 || !p.endOnFullFitness) && !tempGrammar.coveringStarted)
			{
				GA::evolve(tempGrammar);
			}
			if (p.learningMode && p.threads > 1 && (tempGrammar.fitness() < 1.0 || !p.endOnFullFitness))
			{
				exchangeRules();
			}
			step++;
			if (step == nextEmit)
			{
				emit stepChanged(step);
				nextEmit += 20;
			}
		}
		if (step > 20)
		{
			qDebug() << tempGrammar.toString();
		}
		emit stepChanged(p.maxEvolutionSteps);
		if (tempGrammar.fitness() == 1.0)
		{
			successIters++;
			avgSteps += step;
		}
		sum += tempGrammar.fitness();
	}
	stream << "\n\nBest grammar with fitness: " << bestGrammar.fitness() << "\n" << bestGrammar.toString();
	emit iterChanged(p.iterations);
	stream << "\n\naverage fitness: " << QString::number(100.0 * sum / iters, 'f', 1) << "%, max fintess: " << maxFitness;
	stream << "\nsuccess in " << successIters << "/" << iters << " iters\naverage steps to 100%: " << (float)avgSteps / successIters << "/" << maxSteps;
	file.close();
	//	qDebug() << QString() + __FUNCTION__ + " end";
}

void GCS::exchangeRules()
{
	takeRules();
	if (Random::rand() < Params::instance().exchangeProb && Params::instance().exchangeAmount > 0)
	{
		QList<NClassifier> list;
		createRuleSet(list);
		parent.sendRules(list);
	}
}

void GCS::createRuleSet(QList<NClassifier>& list)
{
	switch(Params::instance().exchStrategy)
	{
		case RANDOM:
			for (int i = 0, size = tempGrammar.PN.size(); i < Params::instance().exchangeAmount; ++i)
			{
				list << tempGrammar.PN[Random::rand(size)];
			}
			break;
		case BEST:
			qSort(tempGrammar.PN);
			for (int i = tempGrammar.PN.size()-1, j = 0; j < Params::instance().exchangeAmount; --i, ++j)
			{
				list << tempGrammar.PN[i];
			}
			break;
		case FROM_TERMINAL:
			if (!tempGrammar.PT.isEmpty())
			{
				qSort(tempGrammar.PN);
				NSymbol s;
				s = tempGrammar.PT[Random::rand(tempGrammar.PT.size())].action.symbol;
				for (int i = tempGrammar.PN.size()-1; i >=0; --i)
				{
					NClassifier cl = tempGrammar.PN[i];
					if (cl.condition.firstSymbol == s || cl.condition.secondSymbol == s)
					{
						list << cl;
						break;
					}
				}
				if (!list.isEmpty())
				{
					for (int i = 1; i < Params::instance().exchangeAmount; ++i)
					{
						NClassifier childCl = list[Random::rand(list.size())];
						for (int j = tempGrammar.PN.size()-1; j >= 0; --j)
						{
							NClassifier cl = tempGrammar.PN[j];
							if (!list.contains(cl) && (cl.condition.firstSymbol == childCl.action.symbol || cl.condition.secondSymbol == childCl.action.symbol))
							{
								list << cl;
								break;
							}
						}
					}
				}
			}
			break;
		case FROM_START:
			qSort(tempGrammar.PN);
			for (int i = tempGrammar.PN.size()-1; i >=0; --i)
			{
				NClassifier cl = tempGrammar.PN[i];
				if (cl.action.symbol == tempGrammar.Start)
				{
					list << cl;
					break;
				}
			}
			if (!list.isEmpty())
			{
				for (int i = 1; i < Params::instance().exchangeAmount; ++i)
				{
					NClassifier parentCl = list[Random::rand(list.size())];
					for (int j = tempGrammar.PN.size()-1; j >= 0; --j)
					{
						NClassifier cl = tempGrammar.PN[j];
						if (!list.contains(cl) && (cl.action.symbol == parentCl.condition.firstSymbol || cl.action.symbol == parentCl.condition.secondSymbol))
						{
							list << cl;
							break;
						}
					}
				}
			}
			break;
	}
}

void GCS::putRules(const QList<NClassifier>& list)
{
//	qDebug() << "sendRules";
	QMutexLocker locker(&mutex);
	if (!bufferBusy)
	{
		exchangeBuffer = list;
		bufferBusy = true;
	}
}

void GCS::takeRules()
{
	QMutexLocker locker(&mutex);
	if (bufferBusy)
	{
		for (int i = 0, size = exchangeBuffer.size(); i < size; i++)
		{
			tempGrammar.addClWithCrowding(exchangeBuffer[i], tempGrammar.PN, Params::instance().maxNonterminalRules);
		}
		exchangeBuffer.clear();
		bufferBusy = false;
	}
}

void GCS::nextSentence(int value)
{
	emit parsedSentenceChanged(value);
}

GCS::~GCS()
{
}
