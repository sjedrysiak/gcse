#include "Params.h"

Params Params::mInstance;

Params::Params()
{
	allowCorrection = false;
	allowCoveringStart = true;
	allowCoveringFull = true;
	allowCoveringUniversal = false;
	coveringAggressiveProb = 1.0;
	allowGA = true;
	selectionParent1 = GA::RANDOM;
	selectionParent2 = GA::RANDOM;
	crossoverProb = 0.2;
	mutationProb = 0.2;
	inversionProb = 0.2;
	eliteSize = 2;
	tournamentSize = 2;
	crowdFactor = 2;
	crowdSize = 2;
	baseAmount = 2;
	renouncedAmountFactor = 0.5;
	maxNonterminalRules = 20;
	startNonterminalRules = 5;
	nonterminalSymbolsAmount = 26;
	iterations = 5;
	maxEvolutionSteps = 1;
	positiveSentenceWeight = 1;
	negativeSentenceWeight = 1;
	classicFunWeight = 1;
	fertilityFunWeight = 1;
	unusedClassifierFitness = 0.0;
}
