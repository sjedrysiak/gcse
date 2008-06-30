#include "gcs.h"

GCS::GCS()
{
    //wczytaj zdania do R
    //ustaw parametry modelu
}

void GCS::run()
{
    this->mGrammar.initGrammar();
    while (Params::iterations())
    {
        this->mGrammar.induct(sentences);
        //oblicz fitness dla G
        if (Params::allowGA())
        {
            GA::evolve(this->mGrammar);
        }
    }
}

GCS::~GCS()
{
}
