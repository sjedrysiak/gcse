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

#include "ga.h"
#include "grammar.h"
#include "random.h"
#include "params.h"
#include <QSet>

void GA::evolve(Grammar& g)
{
    if (g.PNSet().isEmpty() || g.PNSet().size() <= Params::eliteSize())
    {
        return;
    }

    NClassifier cl1, cl2;
    switch (Params::selectionCl1())
    {
        case RANDOM:
            cl1 = selectionRandom(g);
            break;
        case ROULETTE:
            cl1 = selectionRoulette(g);
            break;
        case TOURNAMENT:
            cl1 = selectionTournament(g);
            break;
        default:
            cl1 = selectionRoulette(g);
            break;
    }
    switch (Params::selectionCl2())
    {
        case RANDOM:
            cl2 = selectionRandom(g);
            break;
        case ROULETTE:
            cl2 = selectionRoulette(g);
            break;
        case TOURNAMENT:
            cl2 = selectionTournament(g);
            break;
        default:
            cl2 = selectionRoulette(g);
            break;
    }

    //Crossover
    if (Random::rand() < Params::crossoverProb())
    {
        crossover(cl1, cl2);
    }

    //Mutation
    QList<NSymbol> symbols(g.NSet());
    mutation(cl1, symbols);
    mutation(cl2, symbols);

    //Inversion
    if (Random::rand() < Params::inversionProb())
    {
        inversion(cl1);
    }
    if (Random::rand() < Params::inversionProb())
    {
        inversion(cl2);
    }

    //Elite population
    QList<NClassifier> elite = g.PNSet();
    QList<NClassifier> temp;
//    temp.reserve(elite.size() - Params::eliteSize());
    qSort(elite);
    for (int i = elite.size() - Params::eliteSize(); i > 0; i--)
    {
        temp << elite.takeFirst();
    }

    g.addClWithCrowding(cl1, temp);
    g.addClWithCrowding(cl2, temp);
    g.setPN(elite + temp);
}

//selection operators
NClassifier GA::selectionRoulette(const Grammar& g)
{
    double sum = 0.0;
    foreach (NClassifier cl, g.PNSet())
    {
        sum += cl.fitness();
    }
    double random = Random::rand() * sum;
    sum = 0.0;
    NClassifier winner;
    foreach (NClassifier cl, g.PNSet())
    {
        sum += cl.fitness();
        if (random <= sum)
        {
            winner = cl;
            break;
        }
    }
    return winner;
}

NClassifier GA::selectionTournament(const Grammar& g)
{
    NClassifier bestCl;

    QList<NClassifier> PNList(g.PNSet());
    int PNSize = PNList.size();

    if (Params::tournamentSize() >= PNSize)
    {
        bestCl = PNList[0];
        foreach (NClassifier cl, PNList)
        {
            if (cl.fitness() > bestCl.fitness())
            {
                bestCl = cl;
            }
        }
    }
    else
    {
        QSet<NClassifier> tournamentSet;
        bestCl = PNList[Random::rand(PNSize)];
        tournamentSet << bestCl;

        NClassifier cl;
        while (tournamentSet.size() < Params::tournamentSize())
        {
            cl = PNList[Random::rand(PNSize)];
            tournamentSet << cl;
            if (cl.fitness() > bestCl.fitness())
            {
                bestCl = cl;
            }
        }
    }
    return bestCl;
}

NClassifier GA::selectionRandom(const Grammar& g)
{
    return g.PNSet()[Random::rand(g.PNSet().size())];
}

//genetic operators
void GA::crossover(NClassifier& first, NClassifier& second)
{
    NClassifier copyFirst(first);
    if (Random::rand(2) == 1)
    {
        first.setProdAction(NProdAction(second.prodAction().firstSymbol(), first.prodAction().secondSymbol()));
        second.setProdAction(NProdAction(copyFirst.prodAction().firstSymbol(), second.prodAction().secondSymbol()));
    }
    else
    {
        first.setProdAction(NProdAction(first.prodAction().firstSymbol(), second.prodAction().secondSymbol()));
        second.setProdAction(NProdAction(second.prodAction().firstSymbol(), copyFirst.prodAction().secondSymbol()));
    }
    first.setProdCondition(Condition(second.prodCondition().symbol()));
    second.setProdCondition(Condition(copyFirst.prodCondition().symbol()));
}

void GA::inversion(NClassifier& cl)
{
    cl.setProdAction(NProdAction(cl.prodAction().secondSymbol(), cl.prodAction().firstSymbol()));
}

void GA::mutation(NClassifier& cl, const QList<NSymbol>& symbols)
{
    //mutation of the left side
    if (Random::rand() < Params::mutationProb())
    {
        int i = Random::rand(symbols.size());
        cl.setProdCondition(Condition(symbols[i]));
    }
    //mutation of first right symbol
    if (Random::rand() < Params::mutationProb())
    {
        int i = Random::rand(symbols.size());
        cl.setProdAction(NProdAction(symbols[i], cl.prodAction().secondSymbol()));
    }
    //mutation of second right symbol
    if (Random::rand() < Params::mutationProb())
    {
        int i = Random::rand(symbols.size());
        cl.setProdAction(NProdAction(cl.prodAction().firstSymbol(), symbols[i]));
    }
}
