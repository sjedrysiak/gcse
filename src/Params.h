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

#include <QString>
#include "GA.h"
#include "MyException.h"
#include <QMutex>

class Params
{
public:
	static bool allowCorrection();
	static void setAllowCorrection(bool allow);
	static bool allowCoveringStart();
	static void setAllowCoveringStart(bool allow);
	static bool allowCoveringFull();
	static void setAllowCoveringFull(bool allow);
	static bool allowCoveringUniversal();
	static void setAllowCoveringUniversal(bool allow);
	static float coveringAggressiveProb();
	static void setCoveringAggressiveProb(float probability) throw(ArgumentOutOfRangeException);
	static bool allowGA();
	static void setAllowGA(bool allow);
	static GA::SelectionType selectionCl1();
	static void setSelectionCl1(GA::SelectionType selection);
	static GA::SelectionType selectionCl2();
	static void setSelectionCl2(GA::SelectionType selection);
	static float crossoverProb();
	static void setCrossoverProb(float probability) throw(ArgumentOutOfRangeException);
	static float mutationProb();
	static void setMutationProb(float probability) throw(ArgumentOutOfRangeException);
	static float inversionProb();
	static void setInversionProb(float probability) throw(ArgumentOutOfRangeException);
	static int eliteSize();
	static void setEliteSize(int size) throw(ArgumentOutOfRangeException);
	static int tournamentSize();
	static void setTournamentSize(int size) throw(ArgumentOutOfRangeException);
	static int crowdFactor();
	static void setCrowdFactor(int factor) throw(ArgumentOutOfRangeException);
	static int crowdSize();
	static void setCrowdSize(int size) throw(ArgumentOutOfRangeException);
	static int baseAmount();
	static void setBaseAmount(int amount) throw(ArgumentOutOfRangeException);
	static int renouncedAmountFactor();
	static void setRenouncedAmountFactor(int factor) throw(ArgumentOutOfRangeException);
	static int maxPopulationSize();
	static void setMaxPopulationSize(int size) throw(ArgumentOutOfRangeException);
	static int startNonterminalProdsAmount();
	static void setStartNonterminalProdsAmount(int amount) throw(ArgumentOutOfRangeException);
	static int nonterminalSymbolsAmount();
	static void setNonterminalSymbolsAmount(int amount) throw(ArgumentOutOfRangeException);
	static int terminalSymbolsAmount();
	static void setTerminalSymbolsAmount(int amount) throw(ArgumentOutOfRangeException);
	static int iterations();
	static void setIterations(int amount) throw(ArgumentOutOfRangeException);
	static unsigned int maxEvolutionSteps();
	static void setMaxEvolutionSteps(unsigned int steps) throw(ArgumentOutOfRangeException);
	static int positiveSentenceWeight();
	static void setPositiveSentenceWeight(int weight) throw(ArgumentOutOfRangeException);
	static int negativeSentenceWeight();
	static void setNegativeSentenceWeight(int weight) throw(ArgumentOutOfRangeException);
	static int classicFunWeight();
	static void setClassicFunWeight(int weight) throw(ArgumentOutOfRangeException);
	static int fertilityFunWeight();
	static void setFertilityFunWeight(int weight) throw(ArgumentOutOfRangeException);
	static double unusedClassifierFitness();
	static void setUnusedClassifierFitness(double fitness) throw(ArgumentOutOfRangeException);
	static QString sentencesFilePath();
	static void setSentencesFilePath(const QString& path);
private:
	//allow to run grammar correction
	static bool mAllowCorrection;

	//parameters for covering operators
	//allow to run start covering
	static bool mAllowCoveringStart;
	//allow to run full covering
	static bool mAllowCoveringFull;
	//allow to run universal covering
	static bool mAllowCoveringUniversal;
	//aggressive covering probability
	static float mCoveringAggressiveProb;

	//parameters for ga
	//allow to run ga
	static bool mAllowGA;
	//selection type for first classifier
	static GA::SelectionType mSelectionCl1;
	//selection type for second classifier
	static GA::SelectionType mSelectionCl2;
	//crossover probability
	static float mCrossoverProb;
	//mutation probability
	static float mMutationProb;
	//inversion probability
	static float mInversionProb;

	//elite population size [1,30]
	static int mEliteSize;
	//tournament population size [1,30]
	static int mTournamentSize;
	//crowding factor [1,30]
	static int mCrowdFactor;
	//crowding population size [1,30]
	static int mCrowdSize;

	//parsing parameters
	//classifier base amount [0,15]
	static int mBaseAmount;
	//renounced amount factor [0,15]
	static int mRenouncedAmountFactor;
	//classifiers population size ( = mStartNonterminalProdsAmount + PT.size())
	static int mMaxPopulationSize;
	//start nonterminal classifiers amount [1,30]
	static int mStartNonterminalProdsAmount;
	//nonterminal symbols amount [1,30]
	static int mNonterminalSymbolsAmount;
	//terminal symbols amount
	static int mTerminalSymbolsAmount;
	//number of iterations 10, 50
	static int mIterations;
	//max evolution steps [1,50 000]
	static unsigned int mMaxEvolutionSteps;
	//positive sentence weight [1,20]
	static int mPositiveSentenceWeight;
	//negative sentence weight [1,20]
	static int mNegativeSentenceWeight;
	//classic function weight [1,20]
	static int mClassicFunWeight;
	//fertility function weight [1,15]
	static int mFertilityFunWeight;
	//unused classifier fitness [1,10]
	static double mUnusedClassifierFitness;

	static QString mSentencesFilePath;
	static QMutex mutex;
};

#endif /*PARAMS_H_*/
