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

//TODO temporary
#include <QtCore>

bool CYK::parse(const Sentence& sentence, Grammar& g)
{
	int size = sentence.size();
	CYKTable cykTable(size);
	for (int row = 0; row < size; row++) //set rows' sizes
	{
		cykTable[row].resize(size - row);
	}
	QList<Classifier*> M;
	for (int col = 0; col < size; col++) //set first row
	{
		M = getMatchingClassifiers(TCondition(sentence[col]), g, sentence.isPositive());
		if (M.size() == 0) //there is no terminal prod for current word
		{//TODO sprawdzić czy covering terminal jest tylko dla zdań pozytywnych
			M << coveringTerminal(sentence[col], g);
			if (Params::allowCoveringUniversal())
			{
				M << coveringUniversal(sentence[col], g);
			}
			if (size == 1 && sentence.isPositive() && Params::allowCoveringStart())
			{
				M << coveringStart(sentence[col], g);
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
						M << coveringFull(D[i], g);
					}
				}
			}
			cykTable[row][col] += M;
		}
	}
//	for (int i = 0; i < cykTable.size() && i < 1; i++)
//	{
//		for (int j = 0; j < cykTable[i].size(); j++)
//		{
//			for (int k = 0; k < cykTable[i][j].size(); k++)
//			{
//				qDebug() << i << j << ((TClassifier*)cykTable[i][j][k])->operator QString();
//			}
//		}
//	}
//	for (int i = 1; i < cykTable.size(); i++)
//	{
//		for (int j = 0; j < cykTable[i].size(); j++)
//		{
//			for (int k = 0; k < cykTable[i][j].size(); k++)
//			{
//				qDebug() << i << j << k << ((NClassifier*)cykTable[i][j][k])->operator QString();
//			}
//		}
//	}
	for (int i = 0; i < cykTable[size - 1][0].size(); i++)
	{
		if (cykTable[size - 1][0][i]->action().symbol() == g.Start)
		{
			//TODO czy update parametrów za każdym razem czy tylko jeśli się uda sparsować zdanie?
			updateClParams(cykTable, sentence.isPositive());
			return true;
		}
	}
	return false;
}

QList<Classifier*> CYK::getMatchingClassifiers(const NCondition& condition, Grammar& g, bool isPositive)
{
	QList<Classifier*> set;
	for (int i = 0; i < g.PN.size(); i++)
	{
		if (g.PN[i].condition() == condition)
		{
			set << &(g.PN[i]);
			g.PN[i].used(isPositive);
		}
	}
	return set;//.toList();
}

QList<Classifier*> CYK::getMatchingClassifiers(const TCondition& condition, Grammar& g, bool isPositive)
{
	QList<Classifier*> set;
	for (int i = 0; i < g.PT.size(); i++)
	{
		if (g.PT[i].condition() == condition)
		{
			set << &(g.PT[i]);
			g.PT[i].used(isPositive);
		}
	}
	return set;//.toList();
}

QList<NCondition> CYK::getConditionsForCykCell(const CYKTable& cykTable, int row, int col)
{
	QSet<NCondition> set;
	for (int r = 0; r < row; r++)
	{
		for (int i = 0; i < cykTable[r][col].size(); i++)
		{
			Classifier* cl1 = cykTable[r][col][i];
			for (int j = 0; j < cykTable[row - 1 - r][col + 1 + r].size(); j++)
			{
				Classifier* cl2 = cykTable[row - 1 - r][col + 1 + r][j];
				set << NCondition(cl1->action().symbol(), cl2->action().symbol());
			}
		}
	}
	return set.toList();
}

//covering operators
Classifier* CYK::coveringTerminal(const TSymbol& term, Grammar& g)
{
	NSymbol newSymbol = g.N[Random::rand(g.N.size())];
	TCondition cond(term);
	Action act(newSymbol);
	TClassifier cl(cond, act);
	cl.used(true);
	g.addClNormal(cl);

	///////////
	int idx = g.PT.indexOf(cl);
	return &(g.PT[idx]);
}

Classifier* CYK::coveringUniversal(const TSymbol& term, Grammar& g)
{
	TClassifier cl(TCondition(term), Action(g.Universal));
	cl.used(true);
	g.addClNormal(cl);

	///////////
	int idx = g.PT.indexOf(cl);
	return &(g.PT[idx]);
}

Classifier* CYK::coveringStart(const TSymbol& term, Grammar& g)
{
	TClassifier cl(TCondition(term), Action(g.Start));
	cl.used(true);
	Grammar::addClWithCrowding(cl, g.PT);

	///////////
	int idx = g.PT.indexOf(cl);
	return &(g.PT[idx]);
}

Classifier* CYK::coveringFull(const NCondition& cond, Grammar& g)
{
	NClassifier cl(cond, Action(g.Start));
	cl.used(true);
	Grammar::addClWithCrowding(cl, g.PN);

	///////////
	int idx = g.PN.indexOf(cl);
	return &(g.PN[idx]);
}

Classifier* CYK::coveringAggressive(const NCondition& cond, Grammar& g)
{
	NSymbol newSymbol = g.N[Random::rand(g.N.size())];
	NClassifier cl(cond, Action(newSymbol));
	cl.used(true);
	Grammar::addClWithCrowding(cl, g.PN);

	///////////
	int idx = g.PN.indexOf(cl);
	return &(g.PN[idx]);
}

void CYK::updateClParams(CYKTable& cykTable, bool isPositive)
{
	int row = cykTable.size() - 1;
	if (row < 1)//one word sentence (Start -> terminal)
	{
		return;
	}
	int col = 0;
	QList<Classifier*> list = cykTable[row][col];
	for (int i = 0; i < list.size(); i++)
	{
		//TODO update wszystkich czy tylko Start?
		NClassifier* cl = (NClassifier*)list[i];
		int amount = (computeAmount(cykTable, cl->condition().firstSymbol(), row, col, true, isPositive) + computeAmount(cykTable, cl->condition().secondSymbol(), row, col, false, isPositive)) * Params::renouncedAmountFactor();
		cl->increasePoints(isPositive, amount);
	}
}

int CYK::computeAmount(CYKTable& cykTable, const NSymbol& symbol, int row, int col, bool isLeftSymbol, bool isPositive)
{
	if (row < 2)
	{
		return Params::baseAmount();
	}
	NClassifier* cl = NULL;
	int newRow, newCol;
	if (isLeftSymbol == true)//searching vertically
	{
		for (int i = 0; i < row; i++)//go from row 0 to row from function param
		{
			for (int j = 0; j < cykTable[i][col].size(); j++)//go through cell
			{
				if (cykTable[i][col][j]->action().symbol() == symbol)
				{
					cl = (NClassifier*)cykTable[i][col][j];
					newRow = i;
					newCol = col;
					i = row;//for break from outer loop
					break;
				}
			}
		}
	}
	else//searching aslant
	{
		for (int i = 0; i < row; i++)//go from row over row from param to row 0
		{
			for (int j = 0; j < cykTable[row-1-i][col-1-i].size(); j++)//go through cell
			{
				if (cykTable[row-1-i][col-1-i][j]->action().symbol() == symbol)
				{
					cl = (NClassifier*)cykTable[row-1-i][col-1-i][j];
					newRow = row-1-i;
					newCol = col-1-i;
					i = row;
					break;
				}
			}
		}
	}
	int amount = (computeAmount(cykTable, cl->condition().firstSymbol(), newRow, newCol, true, isPositive) + computeAmount(cykTable, cl->condition().secondSymbol(), newRow, newCol, false, isPositive)) * Params::renouncedAmountFactor();
	cl->increasePoints(isPositive, amount);
	return amount;
}
