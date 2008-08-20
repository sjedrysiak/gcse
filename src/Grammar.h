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

#include "Symbol.h"
#include "Classifier.h"
#include <QList>

class Sentence;
class CYK;
class GA;

class Grammar
{
	friend class CYK;
	friend class GA;
public:
	const NSymbol Start;
	const NSymbol Universal;

	Grammar(const NSymbol& start = "_S_", const NSymbol& universal = "_U_");
	void initGrammar(int nonterminals = 0, int rules = 0);
	void induct(const QList<Sentence>& sentences);
	void resetClParams();
	void correction();
	float computeFitness();
	float fitness() const;
	void copyClParameters(const Grammar& other);
	const QList<NSymbol>& NSet() const;
	const QList<TSymbol>& TSet() const;
	const QList<NClassifier>& PNSet() const;
	const QList<TClassifier>& PTSet() const;
	void setN(const QList<NSymbol>& source);
	void setT(const QList<TSymbol>& source);
	void setPN(const QList<NClassifier>& source);
	void setPT(const QList<TClassifier>& source);

	//adding methods
	void addSymbol(const NSymbol& s);
	void addSymbol(const TSymbol& s);
	void addClNormal(const NClassifier& cl);
	void addClNormal(const TClassifier& cl);
	static NClassifier* addClWithCrowding(const NClassifier& newCl, QList<NClassifier>& set);
	static TClassifier* addClWithCrowding(const TClassifier& newCl, QList<TClassifier>& set);

	int maxClPointsDifference() const;
	int minClPointsDifference() const;
	int computeMaxClPointsDifference();
	int computeMinClPointsDifference();

	~Grammar();
private:
	QList<NSymbol> N;
	QList<TSymbol> T;
	QList<NClassifier> PN;
	QList<TClassifier> PT;
	//for fitness computing
	float mFitness;
	unsigned int mNumberOfSentences;
	unsigned int mParsedPositive;
	unsigned int mNotParsedNegative;
	int mMaxClPointsDifference;
	int mMinClPointsDifference;
};

#endif /*GRAMMAR_H_*/