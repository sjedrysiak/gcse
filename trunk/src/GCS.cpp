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
	int avgIter = 0;
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
			if (p.learningMode && p.allowGA && (tempGrammar.fitness() < 1.0 || !p.endOnFullFitness))
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
		emit stepChanged(p.maxEvolutionSteps);
		if (tempGrammar.fitness() == 1.0)
		{
			successIters++;
			avgIter += step-1;
		}
		sum += tempGrammar.fitness();
	}
	stream << "\n\nBest grammar with fitness: " << bestGrammar.fitness() << "\n" << bestGrammar.toString();
	emit iterChanged(p.iterations);
	stream << "\n\naverage fitness: " << QString::number(100.0 * sum / iters, 'f', 1) << "%, max fintess: " << maxFitness;
	stream << "\nsuccess in " << successIters << "/" << iters << " iters\naverage steps to 100%: " << (float)avgIter / successIters << "/" << maxSteps;
	file.close();
	//	qDebug() << QString() + __FUNCTION__ + " end";
}

void GCS::exchangeRules()
{
	takeRules();
	if (Random::rand() < Params::instance().exchangeProb)
	{
		QList<NClassifier> list(tempGrammar.PN);
		qSort(list.begin(), list.end(), qGreater<NClassifier> ());
		for (int i = 0, size = list.size() - Params::instance().exchangeAmount; i < size; i++)
		{
			list.removeLast();
		}
		parent.sendRules(list);
	}
}

void GCS::sendRules(QList<NClassifier> list)
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
