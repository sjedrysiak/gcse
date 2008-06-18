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

#include "cyk.h"
#include "sentence.h"
#include <QString>
#include <QStringList>

bool CYK::mAllowCorrection = true;
bool CYK::mAllowCoveringFull = false;
bool CYK::mAllowCoveringStart = true;
bool CYK::mAllowCoveringUniversal = false;
float CYK::mProbCoveringAggressive = 0.0;

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
        M = getMatchingClassifiers(TProdAction(terminals[col]), g);
        if (M.size() == 0)//there is no terminal prod for current word
        {
            M << coveringTerminal(terminals[col], g);
            if (mAllowCoveringUniversal)
            {
                M << g.Su;
            }
            if (mAllowCoveringStart && size == 1 && sentence.isPositive())
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
                M = getMatchingClassifiers(condition, g);
                if (M.size() == 0 && sentence.isPositive())
                {
                    if (Random::rand() < mProbCoveringAggressive)
                    {
                        M << coveringAggressive(condition, g);
                    }
                    if (mAllowCoveringFull && row == size-1 && col == 0)
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

QList<NSymbol> CYK::getMatchingClassifiers(const NProdAction& condition, const Grammar& g)
{
    QList<NSymbol> list;
    foreach (NClassifier cl, g.PNSet())
    {
        if (cl.condition() == condition)
        {
            list.append( cl.action().symbol() );
        }
    }
    return list;
}

QList<NSymbol> CYK::getMatchingClassifiers(const TProdAction& condition, const Grammar& g)
{
    QList<NSymbol> list;
    foreach (TClassifier cl, g.PTSet())
    {
        if (cl.condition() == condition)
        {
            list.append( cl.action().symbol() );
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
    g.addClNormal(TClassifier(TProdRule(ProdCondition(newSymbol), TProdAction(term))));
    if (mAllowCoveringUniversal)
    {
        coveringUniversal(term, g);
    }
    return newSymbol;
}

void CYK::coveringUniversal(const TSymbol& term, Grammar& g)
{
    g.addClNormal(TClassifier(TProdRule(ProdCondition(g.Su), TProdAction(term))));
}

void CYK::coveringStart(const TSymbol& term, Grammar& g)
{
    g.addClWithCrowding(TClassifier(TProdRule(ProdCondition(g.S), TProdAction(term))), g.PTSet());
}

void CYK::coveringFull(const NProdAction& cond, Grammar& g)
{
    g.addClWithCrowding(NClassifier(NProdRule(ProdCondition(g.S), NProdAction(cond))), g.PNSet());
}

NSymbol CYK::coveringAggressive(const NProdAction& cond, Grammar& g)
{
    NSymbol newSymbol = NSymbol::generateNew();
    g.addClWithCrowding(NClassifier(NProdRule(ProdCondition(newSymbol), NProdAction(cond))), g.PNSet());
    return newSymbol;
}
