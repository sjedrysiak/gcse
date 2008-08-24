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
//	qDebug() << QString() + __FUNCTION__ + " start";
	int size = sentence.size();
	if (size == 0 || g.N.isEmpty())
	{
//		qDebug() << QString() + __FUNCTION__ + " end";
		return false;
	}
	CYKTable cykTable(size);
	for (int row = 0; row < size; row++) //set rows' sizes
	{
		cykTable[row].resize(size - row);
	}
	QList<Classifier*> M;
	for (int col = 0; col < size; col++) //set first row
	{
		M = getMatchingClassifiers(TCondition(sentence[col]), g);
		if (M.isEmpty()) //there is no terminal prod for current word
		{//TODO sprawdzić czy covering terminal jest tylko dla zdań pozytywnych
			M << coveringTerminal(sentence[col], g);
			if (Params::instance().allowCoveringUniversal)
			{
				M << coveringUniversal(sentence[col], g);
			}
			if (size == 1 && sentence.isPositive() && Params::instance().allowCoveringStart)
			{
				M << coveringStart(sentence[col], g);
			}
		}
		cykTable[0][col] = M;
	}

	bool covering;
	do
	{
		covering = false;
		for (int row = 1; row < size; row++)
		{
			for (int col = 0; col < size - row; col++)
			{
				QList<NCondition> D = getConditionsForCykCell(cykTable, row, col);
				M.clear();
				for (int i = 0; i < D.size(); i++)
				{
					M += getMatchingClassifiers(D[i], g);
				}
				if (M.isEmpty() && sentence.isPositive() && !D.isEmpty())
				{
					if (Random::rand() < Params::instance().coveringAggressiveProb)
					{
						M << coveringAggressive(D[0], g);
						covering = true;
					}
					if (Params::instance().allowCoveringFull && row == size - 1 && col == 0)
					{
						M << coveringFull(D[0], g);
						covering = true;
					}
//					if (covering == true)//restart parsing after covering operator with crowding
//					{
//						row = 0;
//						break;
//					}
				}
				cykTable[row][col] = M;
				cykTable[row][col] = cykTable[row][col].toSet().toList();//remove redundant classifiers
			}
		}
	} while (covering == true);
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
//				qDebug() << i << j << ((NClassifier*)cykTable[i][j][k])->operator QString();
//			}
//		}
//	}
	//TODO czy update parametrów za każdym razem czy tylko jeśli się uda sparsować zdanie?
	updateClParams(cykTable, sentence.isPositive());
	for (int i = 0, s = cykTable[size - 1][0].size(); i < s; i++)
	{
		if (cykTable[size - 1][0][i]->action.symbol == g.Start)
		{
//			qDebug() << QString() + __FUNCTION__ + " end";
			return true;
		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return false;
}

QList<Classifier*> CYK::getMatchingClassifiers(const NCondition& condition, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	QList<Classifier*> set;
	for (int i = 0, size = g.PN.size(); i < size; i++)
	{
		if (g.PN[i].condition == condition)
		{
			set << &(g.PN[i]);
		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return set;
}

QList<Classifier*> CYK::getMatchingClassifiers(const TCondition& condition, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	QList<Classifier*> set;
	for (int i = 0, size = g.PT.size(); i < size; i++)
	{
		if (g.PT[i].condition == condition)
		{
			set << &(g.PT[i]);
		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return set;
}

QList<NCondition> CYK::getConditionsForCykCell(const CYKTable& cykTable, int row, int col)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	QSet<NCondition> set;
	if (cykTable.size() > row && cykTable[row].size() > col)
	{
		for (int r = 0; r < row; r++)
		{
			for (int i = 0, size = cykTable[r][col].size(); i < size; i++)//iterate through first cell
			{
				Classifier* cl1 = cykTable[r][col][i];
				for (int j = 0, size2 = cykTable[row - 1 - r][col + 1 + r].size(); j < size2; j++)//iterate through second cell
				{
					Classifier* cl2 = cykTable[row - 1 - r][col + 1 + r][j];
					set << NCondition(cl1->action.symbol, cl2->action.symbol);
				}
			}
		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return set.toList();
}

//covering operators
Classifier* CYK::coveringTerminal(const TSymbol& term, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	NSymbol newSymbol = g.N[Random::rand(g.N.size())];
	TCondition cond(term);
	Action act(newSymbol);
	TClassifier cl(cond, act);
	g.addClNormal(cl);
//	qDebug() << QString() + __FUNCTION__ + " end";
	return &(g.PT.last());
}

Classifier* CYK::coveringUniversal(const TSymbol& term, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	TClassifier cl(TCondition(term), Action(g.Universal));
	g.addClNormal(cl);
//	qDebug() << QString() + __FUNCTION__ + " end";
	return &(g.PT.last());
}

Classifier* CYK::coveringStart(const TSymbol& term, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	TClassifier cl(TCondition(term), Action(g.Start));
	TClassifier* added = Grammar::addClWithCrowding(cl, g.PT, g.PT.size());
//	qDebug() << QString() + __FUNCTION__ + " end";
	return added;
}

Classifier* CYK::coveringFull(const NCondition& cond, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	NClassifier cl(cond, Action(g.Start));
	NClassifier* added = Grammar::addClWithCrowding(cl, g.PN, Params::instance().maxNonterminalRules);
//	qDebug() << QString() + __FUNCTION__ + " end";
	return added;
}

Classifier* CYK::coveringAggressive(const NCondition& cond, Grammar& g)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	NSymbol newSymbol = g.N[Random::rand(g.N.size())];
	NClassifier cl(cond, Action(newSymbol));
	NClassifier* added = Grammar::addClWithCrowding(cl, g.PN, Params::instance().maxNonterminalRules);
//	qDebug() << QString() + __FUNCTION__ + " end";
	return added;
}

void CYK::updateClParams(CYKTable& cykTable, bool isPositive)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	for (int row = 0, s1 = cykTable.size(); row < s1; row++)
	{
		for (int col = 0, s2 = cykTable[row].size(); col < s2; col++)
		{
			for (int i = 0, s3 = cykTable[row][col].size(); i < s3; i++)
			{
				cykTable[row][col][i]->used(isPositive);
			}
		}
	}

	int row = cykTable.size() - 1;
	if (row < 1)//one word sentence (Start -> terminal) or empty sentence
	{
//		qDebug() << QString() + __FUNCTION__ + " end";
		return;
	}
	int col = 0;
	QList<Classifier*> list = cykTable[row][col];
	for (int i = 0, size = list.size(); i < size; i++)
	{
		//TODO update wszystkich czy tylko Start?
		NClassifier* cl = static_cast<NClassifier*>(list[i]);
		int amount = (computeAmount(cykTable, cl->condition.firstSymbol, row, col, true, isPositive) + computeAmount(cykTable, cl->condition.secondSymbol, row, col, false, isPositive)) * Params::instance().renouncedAmountFactor;
		cl->increasePoints(isPositive, amount);
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
}

int CYK::computeAmount(CYKTable& cykTable, const NSymbol& symbol, int row, int col, bool isLeftSymbol, bool isPositive)
{
//	qDebug() << QString() + __FUNCTION__ + " start" << "symbol(" << row << col << "):" << symbol;
	if (row < 2)
	{
//		qDebug() << QString() + __FUNCTION__ + " end";
		return Params::instance().baseAmount;
	}
	NClassifier* cl = NULL;
	int newRow, newCol;
	if (isLeftSymbol == true)//searching vertically
	{
		for (int r = 0; r < row; r++)//go from row 0 to row from function param
		{
			for (int i = 0, size = cykTable[r][col].size(); i < size; i++)//go through cell
			{
				if (cykTable[r][col][i]->action.symbol == symbol)
				{
					if (r == 0) //TClassifier
					{
//						qDebug() << QString() + __FUNCTION__ + " end";
						return Params::instance().baseAmount;
					}
					else
					{
						cl = static_cast<NClassifier*>(cykTable[r][col][i]);
						newRow = r;
						newCol = col;
						r = row;//for break from outer loop
						break;
					}
				}
			}
		}
	}
	else//searching aslant
	{
		for (int r = row-1, c = col+1; r >= 0; r--, c++)//go from row over row from param to row 0
		{
			for (int i = 0, size = cykTable[r][c].size(); i < size; i++)//go through cell
			{
				if (cykTable[r][c][i]->action.symbol == symbol)
				{
					if (r == 0) //TClassifier
					{
//						qDebug() << QString() + __FUNCTION__ + " end";
						return Params::instance().baseAmount;
					}
					else
					{
						cl = static_cast<NClassifier*>(cykTable[r][c][i]);
						newRow = r;
						newCol = c;
						r = -1;
						break;
					}
				}
			}
		}
	}
	int amount = 0;//Params::baseAmount()*2;
	if (cl != NULL)
	{
		amount = (computeAmount(cykTable, cl->condition.firstSymbol, newRow, newCol, true, isPositive) + computeAmount(cykTable, cl->condition.secondSymbol, newRow, newCol, false, isPositive)) * Params::instance().renouncedAmountFactor;
		cl->increasePoints(isPositive, amount);
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return amount;
}
