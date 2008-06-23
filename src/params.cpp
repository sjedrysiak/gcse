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

#include "params.h"
#include "ga.h"

bool Params::mAllowCorrection = false;
bool Params::mAllowCoveringStart = false;
bool Params::mAllowCoveringFull false;
bool Params::mAllowCoveringUniversal false;
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
double Params::mUnusedClassifierFitness  = 0.0;


