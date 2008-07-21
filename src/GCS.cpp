#include "GCS.h"
#include "Params.h"
//#include <QFile>
//#include <QTextStream>
#include <QtCore>

GCS::GCS()
{
	this->setParams();
	this->readSentences(Params::sentencesFilePath());
}

void GCS::run()
{
	this->mGrammar.initGrammar();
	int step = 0;
	while (step < Params::maxEvolutionSteps() && this->mGrammar.fitness() < 1.0)
	{
		this->mGrammar.induct(this->mSentences);
		this->mGrammar.computeFitness();
		if (Params::allowGA())
		{
			GA::evolve(this->mGrammar);
		}
	}
}

void GCS::readSentences(const QString& path)
{
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
}

void GCS::setParams()
{
	Params::setAllowCorrection(false);
	Params::setAllowCoveringStart(true);
	Params::setAllowCoveringFull(true);
	Params::setAllowCoveringUniversal(false);
	Params::setCoveringAggressiveProb(0.0);
	Params::setAllowGA(false);
	Params::setSelectionCl1(GA::RANDOM);
	Params::setSelectionCl2(GA::RANDOM);
	Params::setCrossoverProb(0.0);
	Params::setMutationProb(0.0);
	Params::setInversionProb(0.0);
	Params::setEliteSize(0);
	Params::setTournamentSize(0);
	Params::setCrowdFactor(0);
	Params::setCrowdSize(0);
	Params::setBaseAmount(0);
	Params::setRenouncedAmountFactor(0);
	Params::setPopulationSize(0);
	Params::setStartNonterminalProdsAmount(0);
	Params::setNonterminalSymbolsAmount(0);
	Params::setTerminalSymbolsAmount(0);
	Params::setIterations(0);
	Params::setMaxEvolutionSteps(0);
	Params::setPositiveSentenceWeight(0);
	Params::setNegativeSentenceWeight(0);
	Params::setClassicFunWeight(0);
	Params::setFertilityFunWeight(0);
	Params::setUnusedClassifierFitness(0.0);
	Params::setSentencesFilePath("toy");
}

GCS::~GCS()
{
}
