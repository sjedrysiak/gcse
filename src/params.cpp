#include "params.h"

bool Params::mAllowCorrection = true;
bool Params::mAllowCoveringFull = false;
bool Params::mAllowCoveringStart = true;
bool Params::mAllowCoveringUniversal = false;
float Params::mProbCoveringAggressive = 0.0;
//ga
bool Params::mAllowGA = true;
GA::SelectionType Params::mSelectionCl1 = GA::RANDOM;
GA::SelectionType Params::mSelectionCl2 = GA::RANDOM;
float Params::mProbCrossover = 0.0;
float Params::mProbMutation = 0.0;
float Params::mProbInversion = 0.0;

int Params::mEliteSize = 5;
int Params::mTournamentSize = 5;
int Params::mCrowdFactor = 5;
int Params::mCrowdSize = 5;
int Params::mBaseAmount;
int Params::mRateAmountFactor;
int Params::mPopulationSize;
int Params::mNonterminalProdsStartCount;
int Params::mNonterminalSymbolsCount;
int Params::mTerminalSymbolsCount;
int Params::mIterations;
int Params::mMaxEvolutionSteps;
int Params::mPositiveSentenceWeight;
int Params::mNegativeSentenceWeight;
int Params::mClassicFunWeight;
int Params::mFertilityFunWeight;
double Params::mUnusedClassifierFitness;

