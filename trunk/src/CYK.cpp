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

#include "CYK.h"
#include "Sentence.h"
#include "Grammar.h"
#include "Params.h"
#include "Random.h"
#include <QSet>

bool CYK::parse(const Sentence& sentence, Grammar& g)
{
	int size = sentence.size();
	CYKTable cykTable(size);
	for (int row = 0; row < size; row++) //set rows' sizes
	{
		cykTable[row].resize(size - row);
	}
	QList<NSymbol> M;
	for (int col = 0; col < size; col++) //set first row
	{
		M = getMatchingClassifiers(TCondition(sentence[col]), g, sentence.isPositive());
		if (M.size() == 0) //there is no terminal prod for current word
		{
			M << coveringTerminal(sentence[col], g);
			if (Params::allowCoveringUniversal())
			{
				M << g.Su;
			}
			if (Params::allowCoveringStart() && size == 1 && sentence.isPositive())
			{
				coveringStart(sentence[col], g);
				M << g.S;
			}
		}
		cykTable[0][col] = M;
	}

	for (int row = 1; row < size; row++)
	{
		for (int col = 0; col < size - row; col++)
		{
			QList<NCondition> D = getConditionsForCykCell(cykTable, row, col);
			for (int i = 0; i < D.size(); i++)
			{
				M = getMatchingClassifiers(D[i], g, sentence.isPositive());
				if (M.size() == 0 && sentence.isPositive())
				{
					if (Random::rand() < Params::coveringAggressiveProb())
					{
						M << coveringAggressive(D[i], g);
					}
					if (Params::allowCoveringFull() && row == size - 1 && col == 0)
					{
						coveringFull(D[i], g);
						M << g.S;
					}
				}
			}
			cykTable[row][col] += M;
		}
	}
	//TODO aktualizacja parametrów użytych produkcji
	return cykTable[size - 1][0].contains(g.S);
}

QList<NSymbol> CYK::getMatchingClassifiers(const NCondition& condition, Grammar& g, bool isPositive)
{
	QSet<NSymbol> set;
	for (int i = 0; i < g.PN.size(); i++)
	{
		if (g.PN[i].condition() == condition)
		{
			set << g.PN[i].action().symbol();
			g.PN[i].used(isPositive);//TODO temporary
		}
	}
	return set.toList();
}

QList<NSymbol> CYK::getMatchingClassifiers(const TCondition& condition, Grammar& g, bool isPositive)
{
	QSet<NSymbol> set;
	for (int i = 0; i < g.PT.size(); i++)
	{
		if (g.PT[i].condition() == condition)
		{
			set << g.PT[i].action().symbol();
			g.PN[i].used(isPositive);//TODO temporary
		}
	}
	return set.toList();
}

QList<NCondition> CYK::getConditionsForCykCell(const CYKTable& cykTable, int row, int col)
{
	QSet<NCondition> set;
	for (int r = 0; r < row; r++)
	{
		for (int i = 0; i < cykTable[r][col].size(); i++)
		{
			Action act1 = cykTable[r][col][i];
			for (int j = 0; j < cykTable[row - 1 - r][col + 1 + r].size(); j++)
			{
				Action act2 = cykTable[row - 1 - r][col + 1 + r][j];
				set << NCondition(act1.symbol(), act2.symbol());
			}
		}
	}
	return set.toList();
}

//covering operators
NSymbol CYK::coveringTerminal(const TSymbol& term, Grammar& g)
{
	NSymbol newSymbol;
	//TODO sprawdzić czy ma być nowy symbol czy pobrany losowo z g.N
	TCondition cond(term);
	Action act(newSymbol);
	TClassifier cl(cond, act);
	cl.used(true);
	g.addClNormal(cl);
	if (Params::allowCoveringUniversal())
	{
		coveringUniversal(term, g);
	}
	return newSymbol;
}

void CYK::coveringUniversal(const TSymbol& term, Grammar& g)
{
	TClassifier cl(TCondition(term), Action(g.Su));
	cl.used(true);
	g.addClNormal(cl);
}

void CYK::coveringStart(const TSymbol& term, Grammar& g)
{
	TClassifier cl(TCondition(term), Action(g.S));
	cl.used(true);
	Grammar::addClWithCrowding(cl, g.PT);
}

void CYK::coveringFull(const NCondition& cond, Grammar& g)
{
	NClassifier cl(cond, Action(g.S));
	cl.used(true);
	Grammar::addClWithCrowding(cl, g.PN);
}

NSymbol CYK::coveringAggressive(const NCondition& cond, Grammar& g)
{
	NSymbol newSymbol;
	//TODO sprawdzić czy ma być nowy symbol czy pobrany losowo z g.N
	NClassifier cl(cond, Action(newSymbol));
	cl.used(true);
	Grammar::addClWithCrowding(cl, g.PN);
	return newSymbol;
}
