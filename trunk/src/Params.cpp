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

bool Params::mAllowCorrection = false;
bool Params::mAllowCoveringStart = false;
bool Params::mAllowCoveringFull = false;
bool Params::mAllowCoveringUniversal = false;
float Params::mCoveringAggressiveProb = 0.0;
bool Params::mAllowGA = false;
GA::SelectionType Params::mSelectionCl1 = GA::RANDOM;
GA::SelectionType Params::mSelectionCl2 = GA::RANDOM;
float Params::mCrossoverProb = 0.0;
float Params::mMutationProb = 0.0;
float Params::mInversionProb = 0.0;
int Params::mEliteSize = 0;
int Params::mTournamentSize = 0;
int Params::mCrowdFactor = 0;
int Params::mCrowdSize = 0;
int Params::mBaseAmount = 0;
int Params::mRenouncedAmountFactor = 0;
int Params::mPopulationSize = 0;
int Params::mStartNonterminalProdsAmount = 0;
int Params::mNonterminalSymbolsAmount = 0;
int Params::mTerminalSymbolsAmount = 0;
int Params::mIterations = 0;
int Params::mMaxEvolutionSteps = 0;
int Params::mPositiveSentenceWeight = 0;
int Params::mNegativeSentenceWeight = 0;
int Params::mClassicFunWeight = 0;
int Params::mFertilityFunWeight = 0;
double Params::mUnusedClassifierFitness = 0.0;

bool Params::allowCorrection()
{
	return mAllowCorrection;
}

void Params::setAllowCorrection(bool allow)
{
	mAllowCorrection = allow;
}

bool Params::allowCoveringStart()
{
	return mAllowCoveringStart;
}

void Params::setAllowCoveringStart(bool allow)
{
	mAllowCoveringStart = allow;
}

bool Params::allowCoveringFull()
{
	return mAllowCoveringFull;
}

void Params::setAllowCoveringFull(bool allow)
{
	mAllowCoveringFull = allow;
}

bool Params::allowCoveringUniversal()
{
	return mAllowCoveringUniversal;
}

void Params::setAllowCoveringUniversal(bool allow)
{
	mAllowCoveringUniversal = allow;
}

float Params::coveringAggressiveProb()
{
	return mCoveringAggressiveProb;
}

void Params::setCoveringAggressiveProb(float probability)
{
	if (probability < 0.0 || probability > 1.0)
	{
		//TODO exception argument out of range
		return;
	}
	mCoveringAggressiveProb = probability;
}

bool Params::allowGA()
{
	return mAllowGA;
}

void Params::setAllowGA(bool allow)
{
	mAllowGA = allow;
}

GA::SelectionType Params::selectionCl1()
{
	return mSelectionCl1;
}

void Params::setSelectionCl1(GA::SelectionType selection)
{
	mSelectionCl1 = selection;
}

GA::SelectionType Params::selectionCl2()
{
	return mSelectionCl2;
}

void Params::setSelectionCl2(GA::SelectionType selection)
{
	mSelectionCl2 = selection;
}

float Params::crossoverProb()
{
	return mCrossoverProb;
}

void Params::setCrossoverProb(float probability)
{
	if (probability < 0.0 || probability > 1.0)
	{
		//TODO exception argument out of range
		return;
	}
	mCrossoverProb = probability;
}

float Params::mutationProb()
{
	return mMutationProb;
}

void Params::setMutationProb(float probability)
{
	if (probability < 0.0 || probability > 1.0)
	{
		//TODO exception argument out of range
		return;
	}
	mMutationProb = probability;
}

float Params::inversionProb()
{
	return mInversionProb;
}

void Params::setInversionProb(float probability)
{
	if (probability < 0.0 || probability > 1.0)
	{
		//TODO exception argument out of range
		return;
	}
	mInversionProb = probability;
}

int Params::eliteSize()
{
	return mEliteSize;
}

void Params::setEliteSize(int size)
{
	if (size < 1 || size > 30)
	{
		//TODO exception argument out of range
		return;
	}
	mEliteSize = size;
}

int Params::tournamentSize()
{
	return mTournamentSize;
}

void Params::setTournamentSize(int size)
{
	if (size < 1 || size > 30)
	{
		//TODO exception argument out of range
		return;
	}
	mTournamentSize = size;
}

int Params::crowdFactor()
{
	return mCrowdFactor;
}

void Params::setCrowdFactor(int factor)
{
	if (factor < 1 || factor > 30)
	{
		//TODO exception argument out of range
		return;
	}
	mCrowdFactor = factor;
}

int Params::crowdSize()
{
	return mCrowdSize;
}

void Params::setCrowdSize(int size)
{
	if (size < 1 || size > 30)
	{
		//TODO exception argument out of range
		return;
	}
	mCrowdSize = size;
}

int Params::baseAmount()
{
	return mBaseAmount;
}

void Params::setBaseAmount(int amount)
{
	if (amount < 0 || amount > 15)
	{
		//TODO exception argument out of range
		return;
	}
	mBaseAmount = amount;
}

int Params::renouncedAmountFactor()
{
	return mRenouncedAmountFactor;
}

void Params::setRenouncedAmountFactor(int factor)
{
	if (factor < 0 || factor > 15)
	{
		//TODO exception argument out of range
		return;
	}
	mRenouncedAmountFactor = factor;
}

int Params::populationSize()
{
	//TODO czy to jest w ogóle potrzebne?
	return 0;
}

void Params::setPopulationSize(int size)
{
	size = 0;
	//TODO czy to jest w ogóle potrzebne?
}

int Params::startNonterminalProdsAmount()
{
	return mStartNonterminalProdsAmount;
}

void Params::setStartNonterminalProdsAmount(int amount)
{
	if (amount < 1 || amount > 30)
	{
		//TODO exception argument out of range
		return;
	}
	mStartNonterminalProdsAmount = amount;
}

int Params::nonterminalSymbolsAmount()
{
	return mNonterminalSymbolsAmount;
}

void Params::setNonterminalSymbolsAmount(int amount)
{
	if (amount < 1 || amount > 30)
	{
		//TODO exception argument out of range
		return;
	}
	mNonterminalSymbolsAmount = amount;
}

int Params::terminalSymbolsAmount()
{
	//TODO czy to jest w ogóle potrzebne?
	return 0;
}

void Params::setTerminalSymbolsAmount(int count)
{
	count = 0;
	//TODO czy to jest w ogóle potrzebne?
}

int Params::iterations()
{
	return mIterations;
}

void Params::setIterations(int amount)
{
	if (amount != 10 && amount != 50)
	{
		//TODO exception argument out of range
		return;
	}
	mIterations = amount;
}

int Params::maxEvolutionSteps()
{
	return mMaxEvolutionSteps;
}

void Params::setMaxEvolutionSteps(int steps)
{
	if (steps < 1 || steps > 50000)
	{
		//TODO exception argument out of range
		return;
	}
	mMaxEvolutionSteps = steps;
}

int Params::positiveSentenceWeight()
{
	return mPositiveSentenceWeight;
}

void Params::setPositiveSentenceWeight(int weight)
{
	if (weight < 1 || weight > 20)
	{
		//TODO exception argument out of range
		return;
	}
	mPositiveSentenceWeight = weight;
}

int Params::negativeSentenceWeight()
{
	return mNegativeSentenceWeight;
}

void Params::setNegativeSentenceWeight(int weight)
{
	if (weight < 1 || weight > 20)
	{
		//TODO exception argument out of range
		return;
	}
	mNegativeSentenceWeight = weight;
}

int Params::classicFunWeight()
{
	return mClassicFunWeight;
}

void Params::setClassicFunWeight(int weight)
{
	if (weight < 1 || weight > 20)
	{
		//TODO exception argument out of range
		return;
	}
	mClassicFunWeight = weight;
}

int Params::fertilityFunWeight()
{
	return mFertilityFunWeight;
}

void Params::setFertilityFunWeight(int weight)
{
	if (weight < 1 || weight > 15)
	{
		//TODO exception argument out of range
		return;
	}
	mFertilityFunWeight = weight;
}

double Params::unusedClassifierFitness()
{
	return mUnusedClassifierFitness;
}

void Params::setUnusedClassifierFitness(double fitness)
{
	if (fitness < 0 || fitness > 10)
	{
		//TODO exception argument out of range
		return;
	}
	mUnusedClassifierFitness = fitness;
}
