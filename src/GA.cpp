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
#include "Params.h"

void GA::evolve(Grammar& g)
{
	Params& p = Params::instance();
	if (g.PN.isEmpty() || g.PN.size() <= p.eliteSize)
	{
		return;
	}

	NClassifier cl1(g.PN[0]), cl2(g.PN[0]);
	switch (p.selectionParent1)
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
	switch (p.selectionParent2)
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
	if (Random::rand() < p.crossoverProb)
	{
		crossover(cl1, cl2);
	}

	//Mutation
	mutation(cl1, g.N);
	mutation(cl2, g.N);

	//Inversion
	if (Random::rand() < p.inversionProb)
	{
		inversion(cl1);
	}
	if (Random::rand() < p.inversionProb)
	{
		inversion(cl2);
	}

	//Elite population
	QList<NClassifier> elite;
	QList<NClassifier> temp = g.PN;
	qSort(temp);
	for (int i = 0; i < p.eliteSize; ++i)
	{
		elite << temp.takeLast();
	}

	g.addClWithCrowding(cl1, temp, temp.size());
	g.addClWithCrowding(cl2, temp, temp.size());
	g.PN = temp + elite;
}

//selection operators
NClassifier GA::selectionRoulette(const Grammar& g)
{
	double sum = 0.0;
	for (int i = 0, size = g.PN.size(); i < size; i++)
	{
		sum += g.PN[i].fitness;
	}
	double random = Random::rand() * sum;
	sum = 0.0;
	NClassifier winner(g.PN[0]);
	for (int i = 1, size = g.PN.size(); i < size; i++)
	{
		sum += g.PN[i].fitness;
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
	Params& p = Params::instance();
	int PNSize = g.PN.size();

	if (p.tournamentSize >= PNSize)
	{
		NClassifier bestCl(g.PN[0]);
		for (int i = 0; i < PNSize; i++)
		{
			if (g.PN[i].fitness > bestCl.fitness)
			{
				bestCl = g.PN[i];
			}
		}
		return bestCl;
	}
	else
	{
		QList<NClassifier> tournamentSet;
		NClassifier bestCl(g.PN[Random::rand(PNSize)]);
		tournamentSet << bestCl;
		for (int i = 0; i < p.tournamentSize; i++)
		{
			NClassifier cl(g.PN[Random::rand(PNSize)]);
			tournamentSet << cl;
			if (cl.fitness > bestCl.fitness)
			{
				bestCl = cl;
			}
		}
		return bestCl;
	}
}

//genetic operators
void GA::crossover(NClassifier& first, NClassifier& second)
{
	NClassifier copyFirst(first);
	if (Random::rand(2) == 1)
	{
		first.condition = NCondition(second.condition.firstSymbol, first.condition.secondSymbol);
		second.condition = NCondition(copyFirst.condition.firstSymbol, second.condition.secondSymbol);
	}
	else
	{
		first.condition = NCondition(first.condition.firstSymbol, second.condition.secondSymbol);
		second.condition = NCondition(second.condition.firstSymbol, copyFirst.condition.secondSymbol);
	}
	first.action = Action(second.action.symbol);
	second.action = Action(copyFirst.action.symbol);
	first.fitness = second.fitness = (first.fitness + second.fitness) / 2;
}

void GA::mutation(NClassifier& cl, const QList<NSymbol>& symbols)
{
	Params& p = Params::instance();
	//mutation of first condition symbol
	if (Random::rand() < p.mutationProb)
	{
		int i = Random::rand(symbols.size());
		cl.condition = NCondition(symbols[i], cl.condition.secondSymbol);
	}
	//mutation of second condition symbol
	if (Random::rand() < p.mutationProb)
	{
		int i = Random::rand(symbols.size());
		cl.condition = NCondition(cl.condition.firstSymbol, symbols[i]);
	}
	//mutation of action
	if (Random::rand() < p.mutationProb)
	{
		int i = Random::rand(symbols.size());
		cl.action = Action(symbols[i]);
	}
}
