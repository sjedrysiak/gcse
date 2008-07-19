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
#include "Params.h"
#include "Grammar.h"
#include "Random.h"
#include <QString>
#include <QStringList>

bool CYK::parse(const Sentence& sentence, Grammar& g)
{
    QStringList words(sentence.split(Sentence::wordSeparator(), QString::SkipEmptyParts));
    QList<TSymbol> terminals;
    foreach (QString str, words)
    {
        terminals << TSymbol(str);
    }
    int size = terminals.size();
    CYKTable cykTable(size);
    for (int row = 0; row < size; row++) //set rows' sizes
    {
        cykTable[row].resize(size - row);
    }
    QList<NSymbol> M;//list of matched classifiers' conditions
    for (int col = 0; col < size; col++) //set first row
    {
//        M = getMatchingClassifiers(TProdAction(terminals[col]), g);
        if (M.size() == 0)//there is no terminal prod for current word
        {
            M << coveringTerminal(terminals[col], g);
            if (Params::allowCoveringUniversal())
            {
                M << g.Su;
            }
            if (Params::allowCoveringStart() && size == 1 && sentence.isPositive())
            {
                coveringStart(terminals[col], g);
                M << g.S;
            }
        }
        cykTable[0][col] = M.toSet();
    }

    for (int row = 1; row < size; row++)
    {
        for (int col = 0; col < size - row; col++)
        {
            QList<NProdAction> D = getConditionsForCykCell(cykTable, row, col);
            foreach (NProdAction condition, D)
            {
//                M = getMatchingClassifiers(condition, g);
                if (M.size() == 0 && sentence.isPositive())
                {
                    if (Random::rand() < Params::coveringAggressiveProb())
                    {
                        M << coveringAggressive(condition, g);
                    }
                    if (Params::allowCoveringFull() && row == size-1 && col == 0)
                    {
                        coveringFull(condition, g);
                        M << g.S;
                    }
                }
                cykTable[row][col] += M.toSet();
            }
        }
    }
    return cykTable[size-1][0].contains(g.S);
}

QList<NClassifier> CYK::getMatchingClassifiers(const NProdAction& condition, const Grammar& g)
{
    QList<NClassifier> list;
    foreach (NClassifier cl, g.PNSet())
    {
        if (cl.condition() == condition)
        {
            list.append(cl);
        }
    }
    return list;
}

QList<TClassifier> CYK::getMatchingClassifiers(const TProdAction& condition, const Grammar& g)
{
    QList<TClassifier> list;
    foreach (TClassifier cl, g.PTSet())
    {
        if (cl.condition() == condition)
        {
            list.append(cl);
        }
    }
    return list;
}

QList<NProdAction> CYK::getConditionsForCykCell(const CYKTable& cykTable, int row, int col)
{
    QList<NProdAction> list;
    for (int r = 0; r < row; r++)
    {
        foreach (NSymbol s1, cykTable[r][col])
        {
            foreach (NSymbol s2, cykTable[row-1-r][col+1+r])
            {
                list.append( NProdAction(s1, s2) );
            }
        }
    }
    return list;
}

//covering operators
NSymbol CYK::coveringTerminal(const TSymbol& term, Grammar& g)
{
    NSymbol newSymbol = NSymbol::generateNew();
    g.addClNormal(TClassifier(TProdRule(Condition(newSymbol), TProdAction(term))));
    if (Params::allowCoveringUniversal())
    {
        coveringUniversal(term, g);
    }
    return newSymbol;
}

void CYK::coveringUniversal(const TSymbol& term, Grammar& g)
{
    g.addClNormal(TClassifier(TProdRule(Condition(g.Su), TProdAction(term))));
}

void CYK::coveringStart(const TSymbol& term, Grammar& g)
{
    QList<TClassifier> set = g.PTSet();
    Grammar::addClWithCrowding(TClassifier(TProdRule(Condition(g.S), TProdAction(term))), set);
    g.setPT(set);
}

void CYK::coveringFull(const NProdAction& cond, Grammar& g)
{
	QList<NClassifier> set = g.PNSet();
    Grammar::addClWithCrowding(NClassifier(NProdRule(Condition(g.S), NProdAction(cond))), set);
    g.setPN(set);
}

NSymbol CYK::coveringAggressive(const NProdAction& cond, Grammar& g)
{
    NSymbol newSymbol = NSymbol::generateNew();
    QList<NClassifier> set = g.PNSet();
    Grammar::addClWithCrowding(NClassifier(NProdRule(Condition(newSymbol), NProdAction(cond))), set);
    g.setPN(set);
    return newSymbol;
}