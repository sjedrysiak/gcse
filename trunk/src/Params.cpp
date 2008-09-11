#include "Params.h"

Params Params::mInstance;

Params::Params()
{
	splitLearningSet = true;
	learningMode = true;
	exchangeProb = 0.03;
	exchangeAmount = 5;
	allowCorrection = false;
	allowCoveringStart = true;
	allowCoveringFull = true;
	allowCoveringUniversal = false;
	coveringAggressiveProb = 0.03;

	allowGA = true;
	selectionParent1 = GA::RANDOM;
	selectionParent2 = GA::RANDOM;
	crossoverProb = 0.2;
	mutationProb = 0.8;
	inversionProb = 0.3;
	eliteSize = 3;
	tournamentSize = 3;
	crowdFactor = 18;
	crowdSize = 3;

	baseAmount = 2;
	renouncedAmountFactor = 0.5;
	maxNonterminalRules = 33;
	startNonterminalRules = 30;
	nonterminalSymbolsAmount = 19;

	threads = 1;
	iterations = 50;
	maxEvolutionSteps = 5000;
	endOnFullFitness = true;

	positiveSentenceWeight = 1;
	negativeSentenceWeight = 1;
	classicFunWeight = 1;
	fertilityFunWeight = 0;
	unusedClassifierFitness = 0.0;
}
