/***************************************************************************
 *   Copyright (C) 2008 by Sylwester Jędrysiak                             *
 *   sylwester.jedrysiak@gmail.com                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "Params.h"
#include <QMutexLocker>

bool Params::mAllowCorrection = false;
bool Params::mAllowCoveringStart = true;
bool Params::mAllowCoveringFull = true;
bool Params::mAllowCoveringUniversal = false;
float Params::mCoveringAggressiveProb = 1.0;
bool Params::mAllowGA = false;
GA::SelectionType Params::mSelectionCl1 = GA::RANDOM;
GA::SelectionType Params::mSelectionCl2 = GA::RANDOM;
float Params::mCrossoverProb = 0.0;
float Params::mMutationProb = 0.0;
float Params::mInversionProb = 0.0;
int Params::mEliteSize = 1;
int Params::mTournamentSize = 1;
int Params::mCrowdFactor = 2;
int Params::mCrowdSize = 2;
int Params::mBaseAmount = 5;
int Params::mRenouncedAmountFactor = 2;
int Params::mPopulationSize = 20;
int Params::mStartNonterminalProdsAmount = 5;
int Params::mNonterminalSymbolsAmount = 26;
int Params::mTerminalSymbolsAmount = 1;
int Params::mIterations = 5;
unsigned int Params::mMaxEvolutionSteps = 1;
int Params::mPositiveSentenceWeight = 0;
int Params::mNegativeSentenceWeight = 0;
int Params::mClassicFunWeight = 0;
int Params::mFertilityFunWeight = 0;
double Params::mUnusedClassifierFitness = 0.0;

QString Params::mSentencesFilePath = "";
QMutex Params::mutex;

bool Params::allowCorrection()
{
	QMutexLocker locker(&mutex);
	return mAllowCorrection;
}

void Params::setAllowCorrection(bool allow)
{
	QMutexLocker locker(&mutex);
	mAllowCorrection = allow;
}

bool Params::allowCoveringStart()
{
	QMutexLocker locker(&mutex);
	return mAllowCoveringStart;
}

void Params::setAllowCoveringStart(bool allow)
{
	QMutexLocker locker(&mutex);
	mAllowCoveringStart = allow;
}

bool Params::allowCoveringFull()
{
	QMutexLocker locker(&mutex);
	return mAllowCoveringFull;
}

void Params::setAllowCoveringFull(bool allow)
{
	QMutexLocker locker(&mutex);
	mAllowCoveringFull = allow;
}

bool Params::allowCoveringUniversal()
{
	QMutexLocker locker(&mutex);
	return mAllowCoveringUniversal;
}

void Params::setAllowCoveringUniversal(bool allow)
{
	QMutexLocker locker(&mutex);
	mAllowCoveringUniversal = allow;
}

float Params::coveringAggressiveProb()
{
	QMutexLocker locker(&mutex);
	return mCoveringAggressiveProb;
}

void Params::setCoveringAggressiveProb(float probability) throw(ArgumentOutOfRangeException)
{
	if (probability < 0.0 || probability > 1.0)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Covering aggressive probability value out of range (%1 of %2 - %3)").arg(probability).arg(0.0).arg(1.0));
	}
	QMutexLocker locker(&mutex);
	mCoveringAggressiveProb = probability;
}

bool Params::allowGA()
{
	QMutexLocker locker(&mutex);
	return mAllowGA;
}

void Params::setAllowGA(bool allow)
{
	QMutexLocker locker(&mutex);
	mAllowGA = allow;
}

GA::SelectionType Params::selectionCl1()
{
	QMutexLocker locker(&mutex);
	return mSelectionCl1;
}

void Params::setSelectionCl1(GA::SelectionType selection)
{
	QMutexLocker locker(&mutex);
	mSelectionCl1 = selection;
}

GA::SelectionType Params::selectionCl2()
{
	QMutexLocker locker(&mutex);
	return mSelectionCl2;
}

void Params::setSelectionCl2(GA::SelectionType selection)
{
	QMutexLocker locker(&mutex);
	mSelectionCl2 = selection;
}

float Params::crossoverProb()
{
	QMutexLocker locker(&mutex);
	return mCrossoverProb;
}

void Params::setCrossoverProb(float probability) throw(ArgumentOutOfRangeException)
{
	if (probability < 0.0 || probability > 1.0)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Crossover probability value out of range (%1 of %2 - %3)").arg(probability).arg(0.0).arg(1.0));
	}
	QMutexLocker locker(&mutex);
	mCrossoverProb = probability;
}

float Params::mutationProb()
{
	QMutexLocker locker(&mutex);
	return mMutationProb;
}

void Params::setMutationProb(float probability) throw(ArgumentOutOfRangeException)
{
	if (probability < 0.0 || probability > 1.0)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Mutation probability value out of range (%1 of %2 - %3)").arg(probability).arg(0.0).arg(1.0));
	}
	QMutexLocker locker(&mutex);
	mMutationProb = probability;
}

float Params::inversionProb()
{
	QMutexLocker locker(&mutex);
	return mInversionProb;
}

void Params::setInversionProb(float probability) throw(ArgumentOutOfRangeException)
{
	if (probability < 0.0 || probability > 1.0)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Inversion probability value out of range (%1 of %2 - %3)").arg(probability).arg(0.0).arg(1.0));
	}
	QMutexLocker locker(&mutex);
	mInversionProb = probability;
}

int Params::eliteSize()
{
	QMutexLocker locker(&mutex);
	return mEliteSize;
}

void Params::setEliteSize(int size) throw(ArgumentOutOfRangeException)
{
	if (size < 1 || size > 30)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Elite size value out of range (%1 of %2 - %3)").arg(size).arg(1).arg(30), EXCMSG);
	}
	QMutexLocker locker(&mutex);
	mEliteSize = size;
}

int Params::tournamentSize()
{
	QMutexLocker locker(&mutex);
	return mTournamentSize;
}

void Params::setTournamentSize(int size) throw(ArgumentOutOfRangeException)
{
	if (size < 1 || size > 30)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Tournament size value out of range (%1 of %2 - %3)").arg(size).arg(1).arg(30));
	}
	QMutexLocker locker(&mutex);
	mTournamentSize = size;
}

int Params::crowdFactor()
{
	QMutexLocker locker(&mutex);
	return mCrowdFactor;
}

void Params::setCrowdFactor(int factor) throw(ArgumentOutOfRangeException)
{
	if (factor < 1 || factor > 30)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Crowd factor value out of range (%1 of %2 - %3)").arg(factor).arg(1).arg(30));
	}
	QMutexLocker locker(&mutex);
	mCrowdFactor = factor;
}

int Params::crowdSize()
{
	QMutexLocker locker(&mutex);
	return mCrowdSize;
}

void Params::setCrowdSize(int size) throw(ArgumentOutOfRangeException)
{
	if (size < 1 || size > 30)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Crowd size value out of range (%1 of %2 - %3)").arg(size).arg(1).arg(30));
	}
	QMutexLocker locker(&mutex);
	mCrowdSize = size;
}

int Params::baseAmount()
{
	QMutexLocker locker(&mutex);
	return mBaseAmount;
}

void Params::setBaseAmount(int amount) throw(ArgumentOutOfRangeException)
{
	if (amount < 0 || amount > 15)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Base amount value out of range (%1 of %2 - %3)").arg(amount).arg(0).arg(15));
	}
	QMutexLocker locker(&mutex);
	mBaseAmount = amount;
}

int Params::renouncedAmountFactor()
{
	QMutexLocker locker(&mutex);
	return mRenouncedAmountFactor;
}

void Params::setRenouncedAmountFactor(int factor) throw(ArgumentOutOfRangeException)
{
	if (factor < 0 || factor > 15)
	{
		throw ArgumentOutOfRangeException(QObject::tr("RAF value out of range (%1 of %2 - %3)").arg(factor).arg(0).arg(15));
	}
	QMutexLocker locker(&mutex);
	mRenouncedAmountFactor = factor;
}

int Params::populationSize()
{
	//TODO czy to jest w ogóle potrzebne?
	QMutexLocker locker(&mutex);
	return mPopulationSize;
}

void Params::setPopulationSize(int size) throw(ArgumentOutOfRangeException)
{
	//TODO czy to jest w ogóle potrzebne?
	QMutexLocker locker(&mutex);
	mPopulationSize = size;
}

int Params::startNonterminalProdsAmount()
{
	QMutexLocker locker(&mutex);
	return mStartNonterminalProdsAmount;
}

void Params::setStartNonterminalProdsAmount(int amount) throw(ArgumentOutOfRangeException)
{
	if (amount < 1 || amount > 30)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Start nonterminal productions amount value out of range (%1 of %2 - %3)").arg(amount).arg(1).arg(30));
	}
	QMutexLocker locker(&mutex);
	mStartNonterminalProdsAmount = amount;
}

int Params::nonterminalSymbolsAmount()
{
	QMutexLocker locker(&mutex);
	return mNonterminalSymbolsAmount;
}

void Params::setNonterminalSymbolsAmount(int amount) throw(ArgumentOutOfRangeException)
{
	if (amount < 1 || amount > 30)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Nonterminal symbols amount value out of range (%1 of %2 - %3)").arg(amount).arg(1).arg(30));
	}
	QMutexLocker locker(&mutex);
	mNonterminalSymbolsAmount = amount;
}

int Params::terminalSymbolsAmount()
{
	//TODO czy to jest w ogóle potrzebne?
	QMutexLocker locker(&mutex);
	return mTerminalSymbolsAmount;
}

void Params::setTerminalSymbolsAmount(int amount) throw(ArgumentOutOfRangeException)
{
	//TODO czy to jest w ogóle potrzebne?
	QMutexLocker locker(&mutex);
	mTerminalSymbolsAmount = amount;
}

int Params::iterations()
{
	QMutexLocker locker(&mutex);
	return mIterations;
}

void Params::setIterations(int amount) throw(ArgumentOutOfRangeException)
{
	if (amount != 10 && amount != 50)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Iterations value out of range (%1 of %2 or %3)").arg(amount).arg(10).arg(50));
	}
	QMutexLocker locker(&mutex);
	mIterations = amount;
}

unsigned int Params::maxEvolutionSteps()
{
	QMutexLocker locker(&mutex);
	return mMaxEvolutionSteps;
}

void Params::setMaxEvolutionSteps(unsigned int steps) throw(ArgumentOutOfRangeException)
{
	if (steps < 1 || steps > 50000)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Max evolution steps value out of range (%1 of %2 - %3)").arg(steps).arg(1).arg(50000));
	}
	QMutexLocker locker(&mutex);
	mMaxEvolutionSteps = steps;
}

int Params::positiveSentenceWeight()
{
	QMutexLocker locker(&mutex);
	return mPositiveSentenceWeight;
}

void Params::setPositiveSentenceWeight(int weight) throw(ArgumentOutOfRangeException)
{
	if (weight < 1 || weight > 20)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Positive sentence weight value out of range (%1 of %2 - %3)").arg(weight).arg(1).arg(20));
	}
	QMutexLocker locker(&mutex);
	mPositiveSentenceWeight = weight;
}

int Params::negativeSentenceWeight()
{
	QMutexLocker locker(&mutex);
	return mNegativeSentenceWeight;
}

void Params::setNegativeSentenceWeight(int weight) throw(ArgumentOutOfRangeException)
{
	if (weight < 1 || weight > 20)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Negative sentence weight value out of range (%1 of %2 - %3)").arg(weight).arg(1).arg(20));
	}
	QMutexLocker locker(&mutex);
	mNegativeSentenceWeight = weight;
}

int Params::classicFunWeight()
{
	QMutexLocker locker(&mutex);
	return mClassicFunWeight;
}

void Params::setClassicFunWeight(int weight) throw(ArgumentOutOfRangeException)
{
	if (weight < 1 || weight > 20)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Classic function weight value out of range (%1 of %2 - %3)").arg(weight).arg(1).arg(20));
	}
	QMutexLocker locker(&mutex);
	mClassicFunWeight = weight;
}

int Params::fertilityFunWeight()
{
	QMutexLocker locker(&mutex);
	return mFertilityFunWeight;
}

void Params::setFertilityFunWeight(int weight) throw(ArgumentOutOfRangeException)
{
	if (weight < 1 || weight > 15)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Fertility function weight value out of range (%1 of %2 - %3)").arg(weight).arg(1).arg(15));
	}
	QMutexLocker locker(&mutex);
	mFertilityFunWeight = weight;
}

double Params::unusedClassifierFitness()
{
	QMutexLocker locker(&mutex);
	return mUnusedClassifierFitness;
}

void Params::setUnusedClassifierFitness(double fitness) throw(ArgumentOutOfRangeException)
{
	if (fitness < 0 || fitness > 10)
	{
		throw ArgumentOutOfRangeException(QObject::tr("Unused classifier fitness value out of range (%1 of %2 - %3)").arg(fitness).arg(0.0).arg(10.0));
	}
	QMutexLocker locker(&mutex);
	mUnusedClassifierFitness = fitness;
}

QString Params::sentencesFilePath()
{
	QMutexLocker locker(&mutex);
	return mSentencesFilePath;
}

void Params::setSentencesFilePath(const QString& path)
{
	QMutexLocker locker(&mutex);
	mSentencesFilePath = path;
}
