#include "GCS.h"
#include "Params.h"
//#include <QFile>
//#include <QTextStream>
#include <QtCore>

int GCS::counter = 0;

GCS::GCS(const Grammar& grammar, const QList<Sentence> list) :
	QThread(), mGrammar(grammar), mSentences(list)
{
	threadNumber = counter++;
}

void GCS::run()
{
	//	qDebug() << QString() + __FUNCTION__ + " start";
	Params& p = Params::instance();
	QFile file("thread"+threadNumber);
	file.open(QFile::WriteOnly);
	QTextStream stream(&file);
	int success = 0;
	for (int iter = 0; iter < p.iterations; iter++)
	{
		Grammar tmpGrammar(mGrammar);
//		tmpGrammar = mGrammar;
		stream << "\n\niteration " << iter+1 << " started";
		int step = 0;
		while (step < p.maxEvolutionSteps && (!p.endOnFullFitness || tmpGrammar.fitness() < 1.0))
		{
			tmpGrammar.induct(mSentences);
			tmpGrammar.computeFitness();
			if (tmpGrammar.fitness() == 1.0)
			{
				stream << "\nfitness == 100% in step " << step;
				success++;
			}
			if (p.allowGA && (tmpGrammar.fitness() < 1.0 || !p.endOnFullFitness))
			{
				GA::evolve(tmpGrammar);
			}
			step++;
		}
		stream << "\niteration " << iter << " ended with fitness " << tmpGrammar.fitness();
	}
	stream << "\n\naverage: " << QString::number(100.0 * success / p.iterations,'f',1) << "%";
	file.close();
	//	qDebug() << QString() + __FUNCTION__ + " end";
}

GCS::~GCS()
{
}
