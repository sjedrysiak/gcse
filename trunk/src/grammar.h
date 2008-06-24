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
#include "symbol.h"
#include "classifier.h"

class Grammar
{
public:
    const NSymbol S;
    const NSymbol Su;

    Grammar(const NSymbol& start = "Start", const NSymbol& universal = "Univ");
    void initGrammar();
    void induct();
    const QSet<NSymbol>& NSet() const;
    const QSet<TSymbol>& TSet() const;
    const QSet<NClassifier>& PNSet() const;
    const QSet<TClassifier>& PTSet() const;
    void setN(const QSet<NSymbol>& source);
    void setT(const QSet<TSymbol>& source);
    void setPN(const QSet<NClassifier>& source);
    void setPT(const QSet<TClassifier>& source);

    //adding methods
    void addSymbol(const NSymbol& s);
    void addSymbol(const TSymbol& s);
    void addClNormal(const NClassifier& cl);
    void addClNormal(const TClassifier& cl);
    static void addClWithCrowding(const NClassifier& newCl, QSet<NClassifier>& set);
    static void addClWithCrowding(const TClassifier& newCl, QSet<TClassifier>& set);

    ~Grammar();
private:
    QSet<NSymbol> N;
    QSet<TSymbol> T;
    QSet<NClassifier> PN;
    QSet<TClassifier> PT;
};

#endif /*GRAMMAR_H_*/
