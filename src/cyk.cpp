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
#include <QString>
#include <QStringList>

#include "tsymbol.h"
#include "grammar.h"
#include "tprodaction.h"
//#include "log.h"

CYK::CYK()
{
}

bool CYK::parse(const Grammar& grammar, const QString& sentence, const QString& separator)
{
    QStringList words(sentence.split(separator, QString::SkipEmptyParts));
    QVector<TSymbol> terminals;
    foreach(QString str, words)
    {
        terminals << TSymbol(str);
    }
    int size = terminals.size();
    QVector<QVector<QSet<NSymbol> > > cykTable(size);
    for (int row = 0; row < size; row++) //set rows' sizes
    {
        cykTable[row].resize(size - row);
    }
    for (int col = 0; col < size; col++) //set first row
    {
        cykTable[0][col] = grammar.getConditionsForAction( TProdAction( TSymbol(terminals[col]) ) ).toSet();
    }
    for (int row = 1; row < size; row++)
    {
        for (int col = 0; col < size - row; col++)
        {
            foreach (NProdAction action, getNActionsForCell(cykTable, row, col) )
            {
                cykTable[row][col] += grammar.getConditionsForAction(action).toSet();
            }
        }
    }
    return cykTable[size-1][0].contains(grammar.S);
}

QList<NProdAction> CYK::getNActionsForCell(const QVector<QVector<QSet<NSymbol> > >& cykTable, int row, int col)
{
    QList<NProdAction> actions;
    for (int r = 0; r < row; r++)
    {
        foreach (NSymbol s1, cykTable[r][col])
        {
            foreach (NSymbol s2, cykTable[row-1-r][col+1+r])
            {
                actions.append( NProdAction(s1, s2) );
            }
        }
    }
    return actions;
}

CYK::~CYK()
{
}
