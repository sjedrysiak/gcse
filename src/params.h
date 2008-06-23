#ifndef SETTINGS_H_
#define SETTINGS_H_

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
    static float probCoveringAggressive();
    static void probCoveringAggressive(float probability);
    static bool allowGA();
    static void setAllowGA(bool allow);
    static GA::SelectionType selectionCl1();
    static void setSelectionCl1(GA::SelectionType selection);
    static GA::SelectionType selectionCl2();
    static void setSelectionCl2(GA::SelectionType selection);
    static float probCrossover();
    static void setProbCrossover(float probability);
    static float probMutation();
    static void setProbMutation(float probability);
    static float probInversion();
    static void setProbInversion(float probability);
    static int eliteSize();
    static void setEliteSize(int size);
    static int tournamentSize();
    static void setTournamentSize(int size);
    static int crowdFactor();
    static void setCrowdFactor(int factor);
    static int crowdSize();
    static void setCrowdSize(int size);
    static int baseAmount();
    static void setBaseAmount(int amount);
    static int rateAmountFactor();
    static void setRateAmountFactor(int rate);
    static int populationSize();
    static void setPopulationSize(int size);
    static int nonterminalProdsStartCount();
    static void setNonterminalProdsStartCount(int count);
    static int nonterminalSymbolsCount();
    static void setNonterminalSymbolsCount(int count);
    static int terminalSymbolsCount();
    static void setTerminalSymbolsCount(int count);
    static int iterations();
    static void setIterations(int count);
    static int maxEvolutionSteps();
    static void setMaxEvolutionSteps(int count);
    static int positiveSentenceWeight();
    static void setPositiveSentenceWeight(int weight);
    static int negativeSentenceWeight();
    static void setNegativeSentenceWeight(int weight);
    static int classicFunWeight();
    static void setClassicFunWeight(int weight);
    static int fertilityFunWeight();
    static void setFertilityFunWeight(int weight);
    static double unusedClassifierFitness();
    static void setUnusedClassifierFitness(double fitness);
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
    static float mProbCoveringAggressive;

    //parameters for ga
    //allow to run ga
    static bool mAllowGA;
    //selection type for first classifier
    static GA::SelectionType mSelectionCl1;
    //selection type for second classifier
    static GA::SelectionType mSelectionCl2;
    //crossover probability
    static float mProbCrossover;
    //mutation probability
    static float mProbMutation;
    //inversion probability
    static float mProbInversion;
    //elite population size [1,30]
    //TODO ograniczyc przedzial wartosci
    static int mEliteSize;
    //tournament population size [1,30]
    //TODO ograniczyc przedzial wartosci
    static int mTournamentSize;
    //crowding factor [1,30]
    static int mCrowdFactor;
    //crowding population size [1,30]
    static int mCrowdSize;
    //TODO parametr kwota bazowa ba [0,15]
    static int mBaseAmount;
    //TODO parametr wspolczynnik zmniejszania kwoty bazowej raf [0,15]
    static int mRateAmountFactor;
    //TODO parametr rozmiar populacji klasyfikatorow
    static int mPopulationSize;
    //TODO parametr liczba poczatkowych produkcji nieterminalnych [1,30]
    static int mNonterminalProdsStartCount;
    //TODO parametr liczba symboli nieterminalnych [1,30]
    static int mNonterminalSymbolsCount;
    //TODO parametr liczba symboli terminalnych
    static int mTerminalSymbolsCount;
    //TODO parametr liczba iteracji 10, 50
    static int mIterations;
    //TODO parametr maksymalna liczba krokow ewolucyjnych [1,50 000]
    static int mMaxEvolutionSteps;
    //TODO parametr waga rozbioru zdania poprawnego [1,20]
    static int mPositiveSentenceWeight;
    //TODO parametr waga rozbioru zdania niepoprawnego [1,20]
    static int mNegativeSentenceWeight;
    //TODO parametr waga funkcji klasycznej klasyfikatora [1,20]
    static int mClassicFunWeight;
    //TODO parametr waga funkcji plodnosci klasyfikatora [1,15]
    static int mFertilityFunWeight;
    //TODO parametr miara uzytecznosci klasyfikatora niebioracego udzialu w parsowaniu [1,10]
    static double mUnusedClassifierFitness;
};

#endif /*SETTINGS_H_*/
