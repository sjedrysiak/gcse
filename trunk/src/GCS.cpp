#include "GCS.h"
#include "Params.h"
//#include <QFile>
//#include <QTextStream>
#include <QtCore>

GCS::GCS(const Grammar& grammar, const QList<Sentence> list) :
	QThread(), mGrammar(grammar), mSentences(list)
{
}

void GCS::run()
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	Params& p = Params::instance();
	int step = 0;
	QFile file("wyniki");
	file.open(QFile::WriteOnly);
	QTextStream stream(&file);
	stream << mGrammar;
	double maxFitness = 0;
	while (step < p.maxEvolutionSteps && mGrammar.fitness() < 1.0)
	{
		mGrammar.induct(mSentences);
		mGrammar.computeFitness();
		if (mGrammar.fitness() > maxFitness)
		{
			qDebug() << "step:" << step;
			stream << "\nstep: " << step << " fitness: " << mGrammar.fitness() << "\n\n";
			stream << mGrammar;
			stream.flush();
			maxFitness = mGrammar.fitness();
		}
		if (mGrammar.fitness() < 1.0 && p.allowGA)
		{
			GA::evolve(mGrammar);
		}
		step++;
	}
	stream << "\n\n" << mGrammar;
	file.close();
//	qDebug() << QString() + __FUNCTION__ + " end";
}

GCS::~GCS()
{
}
