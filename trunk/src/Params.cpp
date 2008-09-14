#include "Params.h"

Params Params::mInstance;

Params::Params()
{
	splitLearningSet = false;
	learningMode = true;
	exchangeProb = 0.0;
	exchangeAmount = 0;
	exchStrategy = GCS::FROM_START;

	allowCorrection = false;
	allowCoveringStart = true;
	allowCoveringFull = true;
	allowCoveringUniversal = false;
	coveringAggressiveProb = 0.0;

	allowGA = true;
	selectionParent1 = GA::ROULETTE;
	selectionParent2 = GA::ROULETTE;
	crossoverProb = 0.2;
	mutationProb = 0.8;
	inversionProb = 0.0;
	eliteSize = 0;
	tournamentSize = 3;
	crowdFactor = 18;
	crowdSize = 3;

	baseAmount = 1;
	renouncedAmountFactor = 0.5;
	maxNonterminalRules = 35;
	startNonterminalRules = 30;
	nonterminalSymbolsAmount = 20;

	threads = 1;
	iterations = 50;
	maxEvolutionSteps = 5000;
	endOnFullFitness = true;

	positiveSentenceWeight = 1;
	negativeSentenceWeight = 2;
	classicFunWeight = 1;
	fertilityFunWeight = 0;
	unusedClassifierFitness = 0.5;
}
