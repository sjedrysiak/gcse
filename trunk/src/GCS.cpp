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
	unsigned int step = 0;
	QFile file("wyniki");
	file.open(QFile::WriteOnly);
	QTextStream stream(&file);
	stream << mGrammar;
	double maxFitness = 0;
	while (step < Params::maxEvolutionSteps() && this->mGrammar.fitness() < 1.0)
	{
//		qDebug() << "step:" << step;
		this->mGrammar.induct(this->mSentences);
		this->mGrammar.computeFitness();
		if (mGrammar.fitness() > maxFitness)
		{
			stream << "\nstep: " << step << " fitness: " << mGrammar.fitness() << "\n\n";
			stream << mGrammar;
			stream.flush();
			maxFitness = mGrammar.fitness();
		}
		if (Params::allowGA())
		{
			GA::evolve(this->mGrammar);
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
