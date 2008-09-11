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
class GCS;

class Grammar
{
	friend class CYK;
	friend class GA;
public:
	NSymbol Start;
	NSymbol Universal;

	Grammar(const NSymbol& start = "_S_", const NSymbol& universal = "_U_") :
		Start(start), Universal(universal)
	{
		initGrammar();
	}
	void initGrammar(int nonterminals = 0, int rules = 0);
	void induct(const QList<Sentence>& sentences, GCS& parent);
	void resetClParams();
	void correction();
	float computeFitness()
	{
		//qDebug() << QString() + __FUNCTION__ + " start";
		if (mNumberOfSentences > 0)
		{
			mFitness = (float) (mParsedPositive + mNotParsedNegative) / mNumberOfSentences;
			//qDebug() << "fitness =" << mParsedPositive << "+" << mNotParsedNegative << "/" << mNumberOfSentences;
		}
		//qDebug() << QString() + __FUNCTION__ + " end";
		return mFitness;
	}
	float fitness() const
	{
		return mFitness;
	}
	void clearFitness()
	{
		mFitness = 0;
	}
	//	void copyClParameters(const Grammar& other);

	//adding methods
	bool addSymbol(const NSymbol& s)
	{
		//qDebug() << QString() + __FUNCTION__ + " start";
		if (!N.contains(s))
		{
			N << s;
			return true;
		}
		//qDebug() << QString() + __FUNCTION__ + " end";
		return false;
	}
	bool addSymbol(const TSymbol& s)
	{
		//qDebug() << QString() + __FUNCTION__ + " start";
		if (!T.contains(s))
		{
			T << s;
			return true;
		}
		//qDebug() << QString() + __FUNCTION__ + " end";
		return false;
	}
	bool addClNormal(const NClassifier& cl)
	{
		//qDebug() << QString() + __FUNCTION__ + " start";
		if (!PN.contains(cl))
		{
			//addSymbol(cl.condition.firstSymbol);
			//addSymbol(cl.condition.secondSymbol);
			//addSymbol(cl.action.symbol);
			PN << cl;
			return true;
		}
		//qDebug() << QString() + __FUNCTION__ + " end";
		return false;
	}
	bool addClNormal(const TClassifier& cl)
	{
		//qDebug() << QString() + __FUNCTION__ + " start";
		if (!PT.contains(cl))
		{
			addSymbol(cl.condition.symbol);
			addSymbol(cl.action.symbol);
			PT << cl;
			return true;
		}
		//qDebug() << QString() + __FUNCTION__ + " end";
		return false;
	}
	static NClassifier* addClWithCrowding(const NClassifier& newCl, QList<NClassifier>& set, int maxSize);
	static TClassifier* addClWithCrowding(const TClassifier& newCl, QList<TClassifier>& set, int maxSize);

	QString toString() const;
	~Grammar()
	{
	}

	QList<NSymbol> N;
	QList<TSymbol> T;
	QList<NClassifier> PN;
	QList<TClassifier> PT;
private:
	int maxClPointsDifference() const
	{
		return mMaxClPointsDifference;
	}
	int minClPointsDifference() const
	{
		return mMinClPointsDifference;
	}
	int computeMaxClPointsDifference();
	int computeMinClPointsDifference();

	//for fitness computing
	float mFitness;
	int mNumberOfSentences;
	int mParsedPositive;
	int mNotParsedNegative;
	int mMaxClPointsDifference;
	int mMinClPointsDifference;
};

#endif /*GRAMMAR_H_*/
