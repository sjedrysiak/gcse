#include "GCS.h"
#include "Params.h"
//#include <QFile>
//#include <QTextStream>
#include <QtCore>

GCS::GCS() :
	QThread()
{
	this->setParams();
	this->readSentences(Params::sentencesFilePath());
}

void GCS::run()
{
	qDebug() << QString() + __FUNCTION__ + " start";
	unsigned int step = 0;
	while (step < Params::maxEvolutionSteps() && this->mGrammar.fitness() < 1.0)
	{
		this->mGrammar.induct(this->mSentences);
		this->mGrammar.computeFitness();
		if (Params::allowGA())
		{
			GA::evolve(this->mGrammar);
		}
		step++;
	}
	qDebug() << QString() + __FUNCTION__ + " end";
}

void GCS::readSentences(const QString& path)
{
	qDebug() << QString() + __FUNCTION__ + " start";
	QFile file(path);
	if (file.open(QFile::ReadOnly))
	{
		QTextStream lines(&file);
		while (!lines.atEnd())
		{
			QString tmp = lines.readLine();
			if (tmp.size() > 4)
			{
				QTextStream line(&tmp);
				Sentence snt;
				int positive;
				line >> positive;
				if (positive == 0)
				{
					snt.setPositive(false);
				}
				else
				{
					snt.setPositive(true);
				}
				line.skipWhiteSpace();
				line >> positive;//only for read second number from line (number useless at this moment)
				line.skipWhiteSpace();
				snt = line.readAll();
				this->mSentences << snt;
			}
		}
		file.close();
	}
	//TODO obsługa błędów z plikiem
	qDebug() << QString() + __FUNCTION__ + " end";
}

void GCS::setParams()
{
	qDebug() << QString() + __FUNCTION__ + " start";
	try
	{
		Params::setAllowCorrection(false);
		Params::setAllowCoveringStart(true);
		Params::setAllowCoveringFull(true);
		Params::setAllowCoveringUniversal(false);
		Params::setCoveringAggressiveProb(1.0);//testing
		Params::setAllowGA(false);
		Params::setSelectionCl1(GA::RANDOM);
		Params::setSelectionCl2(GA::RANDOM);
		Params::setCrossoverProb(0.0);
		Params::setMutationProb(0.0);
		Params::setInversionProb(0.0);
		Params::setEliteSize(1);
		Params::setTournamentSize(1);
		Params::setCrowdFactor(1);
		Params::setCrowdSize(1);
		Params::setBaseAmount(0);
		Params::setRenouncedAmountFactor(0);
		Params::setPopulationSize(0);
		Params::setStartNonterminalProdsAmount(1);
		Params::setNonterminalSymbolsAmount(1);
		Params::setTerminalSymbolsAmount(0);
		Params::setIterations(10);
		Params::setMaxEvolutionSteps(50);//testing
		Params::setPositiveSentenceWeight(1);
		Params::setNegativeSentenceWeight(1);
		Params::setClassicFunWeight(1);
		Params::setFertilityFunWeight(1);
		Params::setUnusedClassifierFitness(0.0);
		Params::setSentencesFilePath("toy");
	}
	catch(ArgumentOutOfRangeException& exc)
	{
		qDebug() << exc;
		throw exc;
	}
	catch(MyException& exc)
	{
		qDebug() << exc;
		throw exc;
	}
	qDebug() << QString() + __FUNCTION__ + " end";
}

GCS::~GCS()
{
}
