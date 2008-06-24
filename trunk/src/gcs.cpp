#include "gcs.h"

GCS::GCS()
{
}

void GCS::run()
{
    //wczytaj zdania do R
    //ustaw parametry modelu
    //initGrammar
    while(Params::iterations())
    {
        //indukuj gramatyke
        //oblicz fitness dla G
        if (Params::allowGA())
        {
            GA::evolve(Grammar());
        }
    }
}

GCS::~GCS()
{
}
