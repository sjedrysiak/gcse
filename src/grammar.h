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

#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <QSet>
#include <QVector>
#include "symbol.h"
#include "classifier.h"

typedef QVector<QVector<QSet<NSymbol> > > CYKTable;

class Grammar
{
public:
    const NSymbol S;
    const NSymbol Su;

    Grammar(const NSymbol& start = "Start", const NSymbol& universal = "Univ");
    void generateGrammar();
    bool parse(const QString& sentence, const QString& separator);
    ~Grammar();
private:
    //for CYK
    QList<NSymbol> getMatchingClassifiers(const NProdAction& condition) const;
    QList<NSymbol> getMatchingClassifiers(const TProdAction& condition) const;
    QList<NProdAction> getConditionsForCykCell(const CYKTable& cykTable, int row, int col) const;

    //covering operators
    NSymbol coveringTerminal(const TSymbol& term);
    void coveringUniversal(const TSymbol& term);
    void coveringStart(const TSymbol& term);
    void coveringFull(const NProdAction& cond);
    NSymbol coveringAggressive(const NProdAction& cond);

    //adding methods
    void addSymbol(const NSymbol& s);
    void addSymbol(const TSymbol& s);
    void addClNormal(const NClassifier& cl);
    void addClNormal(const TClassifier& cl);
    void addClWithCrowding(const NClassifier& newCl, QSet<NClassifier>& set);
    void addClWithCrowding(const TClassifier& newCl, QSet<TClassifier>& set);
    //void addWithCrowdingElite(const NClassifier& cl);

    //AG part
    //selection
    NClassifier selectionRoulette() const;
    NClassifier selectionTournament() const;
    NClassifier selectionRandom() const;
    //operators
    void gaCrossover(NClassifier& first, NClassifier& second) const;
    void gaInversion(NClassifier& cl) const;
    void gaMutation(NClassifier& cl) const;
    void ga();

    QSet<NSymbol> N;
    QSet<TSymbol> T;
    QSet<NClassifier> PN;
    QSet<TClassifier> PT;
};

#endif /*GRAMMAR_H_*/
