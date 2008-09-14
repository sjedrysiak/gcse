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

#ifndef PARAMS_H_
#define PARAMS_H_

#include "GA.h"
#include "GCS.h"

class Params
{
public:
	static Params& instance()
	{
		return mInstance;
	}

	bool splitLearningSet;
	bool learningMode;
	float exchangeProb;
	int exchangeAmount;
	GCS::RuleExchangeStrategy exchStrategy;
	//allow to run grammar correction
	bool allowCorrection;

	//parameters for covering operators
	//allow to run start covering
	bool allowCoveringStart;
	//allow to run full covering
	bool allowCoveringFull;
	//allow to run universal covering
	bool allowCoveringUniversal;
	//aggressive covering probability
	float coveringAggressiveProb;

	//parameters for ga
	//allow to run ga
	bool allowGA;
	//selection type for first classifier
	GA::SelectionType selectionParent1;
	//selection type for second classifier
	GA::SelectionType selectionParent2;
	//crossover probability
	float crossoverProb;
	//mutation probability
	float mutationProb;
	//inversion probability
	float inversionProb;
	//elite population size [1,30]
	int eliteSize;
	//tournament population size [1,30]
	int tournamentSize;
	//crowding factor [1,30]
	int crowdFactor;
	//crowding population size [1,30]
	int crowdSize;


	//parsing parameters
	//classifier base amount [0,15]
	int baseAmount;
	//renounced amount factor [0,15]
	float renouncedAmountFactor;
	//max nonterminal rules
	int maxNonterminalRules;
	//start nonterminal classifiers amount [1,30]
	int startNonterminalRules;
	//nonterminal symbols amount [1,30]
	int nonterminalSymbolsAmount;

	//number of threads
	int threads;
	//number of iterations 10, 50
	int iterations;
	//max evolution steps [1,50 000]
	int maxEvolutionSteps;
	//end on 100% fitness
	bool endOnFullFitness;

	//positive sentence weight [1,20]
	int positiveSentenceWeight;
	//negative sentence weight [1,20]
	int negativeSentenceWeight;
	//classic function weight [1,20]
	int classicFunWeight;
	//fertility function weight [1,15]
	int fertilityFunWeight;
	//unused classifier fitness [1,10]
	float unusedClassifierFitness;
private:
	Params();
	Params(const Params&);
	Params& operator=(const Params&);
	static Params mInstance;
};

#endif /*PARAMS_H_*/
