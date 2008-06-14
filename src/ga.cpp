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

//static fields initialization
bool GA::fGA = true;
GA::SelectionType GA::selectionCl1 = GA::RANDOM;
GA::SelectionType GA::selectionCl2 = GA::RANDOM;
float GA::pCrossover = 0.0;
float GA::pMutation = 0.0;
float GA::pInversion = 0.0;
int GA::eliteSize = 5;
int GA::tournamentSize = 5;

void GA::evolve(Grammar& g)
{
    if (g.PNSet().isEmpty() || g.PNSet().size() <= eliteSize)
    {
        return;
    }

    NClassifier cl1, cl2;
    switch (selectionCl1)
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
    switch (selectionCl2)
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
    if (Random::rand() < pCrossover)
    {
        crossover(cl1, cl2);
    }

    //Mutation
    mutation(cl1, g);
    mutation(cl2, g);

    //Inversion
    if (Random::rand() < pInversion)
    {
        inversion(cl1);
    }
    if (Random::rand() < pInversion)
    {
        inversion(cl2);
    }

    //Elite population
    QList<NClassifier> elite = g.PNSet().toList();
    QSet<NClassifier> temp;
    temp.reserve(elite.size() - eliteSize);
    qSort(elite);
    for (int i = elite.size() - eliteSize; i > 0; i--)
    {
        temp += elite.takeFirst();
    }

    g.addClWithCrowding(cl1, temp);
    g.addClWithCrowding(cl2, temp);
    g.setPN(elite.toSet() + temp);
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

    QList<NClassifier> PNList(g.PNSet().toList() );
    int PNSize = PNList.size();

    if (tournamentSize >= PNSize)
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
        while (tournamentSet.size() < tournamentSize)
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
    return g.PNSet().toList()[Random::rand(g.PNSet().size())];
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
    first.setProdCondition(ProdCondition(second.prodCondition().symbol()));
    second.setProdCondition(ProdCondition(copyFirst.prodCondition().symbol()));
}

void GA::inversion(NClassifier& cl)
{
    cl.setProdAction(NProdAction(cl.prodAction().secondSymbol(), cl.prodAction().firstSymbol()));
}

void GA::mutation(NClassifier& cl, const Grammar& g)
{
    QList<NSymbol> symbols(g.NSet().toList());

    //mutation of the left side
    if (Random::rand() < pMutation)
    {
        int i = Random::rand(symbols.size());
        cl.setProdCondition(ProdCondition(symbols[i]));
    }
    //mutation of first right symbol
    if (Random::rand() < pMutation)
    {
        int i = Random::rand(symbols.size());
        cl.setProdAction(NProdAction(symbols[i], cl.prodAction().secondSymbol()));
    }
    //mutation of second right symbol
    if (Random::rand() < pMutation)
    {
        int i = Random::rand(symbols.size());
        cl.setProdAction(NProdAction(cl.prodAction().firstSymbol(), symbols[i]));
    }
}
