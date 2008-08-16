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
	while (step < Params::maxEvolutionSteps() && this->mGrammar.fitness() < 1.0)
	{
		this->mGrammar.induct(this->mSentences);
		qDebug() << this->mGrammar.computeFitness();
		if (Params::allowGA())
		{
			GA::evolve(this->mGrammar);
		}
		step++;
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
}

GCS::~GCS()
{
}
