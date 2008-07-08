#include "gcs.h"
#include "params.h"

GCS::GCS()
{
    //wczytaj zdania do R
    //ustaw parametry modelu
}

void GCS::run()
{
    this->mGrammar.initGrammar();
    int step = 0;
    while (step < Params::maxEvolutionSteps() && this->mGrammar.fintess() < 1.0)
    {
        this->mGrammar.induct(this->mSentences);
        this->mGrammar.computeFitness();
        if (Params::allowGA())
        {
            GA::evolve(this->mGrammar);
        }
    }
}

GCS::~GCS()
{
}
