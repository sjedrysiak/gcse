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

int GCS::counter = 0;

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
	int avgIter = 0;
	int successIters = 0;
	for (int iter = 0; iter < p.iterations; iter++)
	{
		outGrammar = initGrammar;
		stream << "\niteration_" << iter + 1;
		int step = 0;
		stream << "," + QString::number(outGrammar.fitness());
		while (step < p.maxEvolutionSteps && (!p.endOnFullFitness || outGrammar.fitness() < 1.0))
		{
			outGrammar.induct(mSentences);
			outGrammar.computeFitness();
			stream << "," + QString::number(outGrammar.fitness());
			if (p.allowGA && (outGrammar.fitness() < 1.0 || !p.endOnFullFitness))
			{
				GA::evolve(outGrammar);
			}
			if (p.threads > 1 && (outGrammar.fitness() < 1.0 || !p.endOnFullFitness))
			{
				exchangeRules();
			}
			step++;
		}
		if (outGrammar.fitness() == 1.0)
		{
			successIters++;
			avgIter += step-1;
		}
		sum += outGrammar.fitness();
	}
	stream << "\n\naverage fitness: " << QString::number(100.0 * sum / p.iterations, 'f', 1) << "%";
	stream << "\nsuccess in " << successIters << "/" << p.iterations << " iters\naverage steps to 100%: " << (float)avgIter / successIters << "/" << p.maxEvolutionSteps;
	file.close();
	//	qDebug() << QString() + __FUNCTION__ + " end";
}

void GCS::exchangeRules()
{
	takeRules();
	if (Random::rand() < Params::instance().exchangeProb)
	{
		QList<NClassifier> list(outGrammar.PN);
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
			outGrammar.addClWithCrowding(exchangeBuffer[i], outGrammar.PN, Params::instance().maxNonterminalRules);
		}
		exchangeBuffer.clear();
		bufferBusy = false;
	}
}

GCS::~GCS()
{
	QMutexLocker locker(&mutex);
}
