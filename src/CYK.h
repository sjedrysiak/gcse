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

#ifndef CYK_H_
#define CYK_H_

#include "Symbol.h"
#include "Classifier.h"
#include <QVector>
#include <QList>

class Sentence;
class Grammar;

typedef QVector<QVector<QList<Classifier*> > > CYKTable;

class CYK
{
public:
	static bool parse(const Sentence& sentence, Grammar& g);
private:
    static QList<Classifier*> getMatchingClassifiers(const NCondition& condition, Grammar& g, bool isPositive);
    static QList<Classifier*> getMatchingClassifiers(const TCondition& condition, Grammar& g, bool isPositive);
    static QList<NCondition> getConditionsForCykCell(const CYKTable& cykTable, int row, int col);

    //covering operators
    static Classifier* coveringTerminal(const TSymbol& term, Grammar& g);
    static Classifier* coveringUniversal(const TSymbol& term, Grammar& g);
    static Classifier* coveringStart(const TSymbol& term, Grammar& g);
    static Classifier* coveringFull(const NCondition& cond, Grammar& g);
    static Classifier* coveringAggressive(const NCondition& cond, Grammar& g);
    static void updateClParams(CYKTable& cykTable, bool isPositive);
    static int computeAmount(CYKTable& cykTable, const NSymbol& symbol, int row, int col, bool left, bool isPositive);
};

#endif /*CYK_H_*/
