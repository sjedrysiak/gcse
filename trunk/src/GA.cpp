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

#include "GA.h"
#include "Grammar.h"
#include "Random.h"
#include "Params.h"
#include <QSet>

void GA::evolve(Grammar& g)
{
	if (g.PN.isEmpty() || g.PN.size() <= Params::eliteSize())
	{
		return;
	}

	NClassifier cl1(g.PN[0]), cl2(g.PN[0]);
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
	mutation(cl1, g.NSet());
	mutation(cl2, g.NSet());

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
	qSort(elite);
	for (int i = elite.size() - Params::eliteSize(); i > 0; i--)
	{
		temp << elite.takeFirst();
	}

	g.addClWithCrowding(cl1, temp);
	g.addClWithCrowding(cl2, temp);
	g.PN = elite + temp;
}

//selection operators
NClassifier GA::selectionRoulette(const Grammar& g)
{
	double sum = 0.0;
	for (int i = 0; i < g.PN.size(); i++)
	{
		sum += g.PN[i].fitness();
	}
	double random = Random::rand() * sum;
	sum = 0.0;
	NClassifier winner(g.PN[0]);
	for (int i = 1; i < g.PN.size(); i++)
	{
		sum += g.PN[i].fitness();
		if (random <= sum)
		{
			winner = g.PN[i];
			break;
		}
	}
	return winner;
}

NClassifier GA::selectionTournament(const Grammar& g)
{
	int PNSize = g.PN.size();

	if (Params::tournamentSize() >= PNSize)
	{
		NClassifier bestCl(g.PN[0]);
		for (int i = 0; i < PNSize; i++)
		{
			if (g.PN[i].fitness() > bestCl.fitness())
			{
				bestCl = g.PN[i];
			}
		}
		return bestCl;
	}
	else
	{
		QSet<NClassifier> tournamentSet;
		NClassifier bestCl(g.PN[Random::rand(PNSize)]);
		tournamentSet << bestCl;
		while (tournamentSet.size() < Params::tournamentSize())
		{
			NClassifier cl(g.PN[Random::rand(PNSize)]);
			tournamentSet << cl;
			if (cl.fitness() > bestCl.fitness())
			{
				bestCl = cl;
			}
		}
		return bestCl;
	}
}

NClassifier GA::selectionRandom(const Grammar& g)
{
	return g.PN[Random::rand(g.PN.size())];
}

//genetic operators
void GA::crossover(NClassifier& first, NClassifier& second)
{
	NClassifier copyFirst(first);
	if (Random::rand(2) == 1)
	{
		first.setCondition(NCondition(second.condition().firstSymbol(), first.condition().secondSymbol()));
		second.setCondition(NCondition(copyFirst.condition().firstSymbol(), second.condition().secondSymbol()));
	}
	else
	{
		first.setCondition(NCondition(first.condition().firstSymbol(), second.condition().secondSymbol()));
		second.setCondition(NCondition(second.condition().firstSymbol(), copyFirst.condition().secondSymbol()));
	}
	first.setAction(Action(second.action().symbol()));
	second.setAction(Action(copyFirst.action().symbol()));
}

void GA::inversion(NClassifier& cl)
{
	cl.setCondition(NCondition(cl.condition().secondSymbol(), cl.condition().firstSymbol()));
}

void GA::mutation(NClassifier& cl, const QList<NSymbol>& symbols)
{
	//mutation of first condition symbol
	if (Random::rand() < Params::mutationProb())
	{
		int i = Random::rand(symbols.size());
		cl.setCondition(NCondition(symbols[i], cl.condition().secondSymbol()));
	}
	//mutation of second condition symbol
	if (Random::rand() < Params::mutationProb())
	{
		int i = Random::rand(symbols.size());
		cl.setCondition(NCondition(cl.condition().firstSymbol(), symbols[i]));
	}
	//mutation of action
	if (Random::rand() < Params::mutationProb())
	{
		int i = Random::rand(symbols.size());
		cl.setAction(Action(symbols[i]));
	}
}