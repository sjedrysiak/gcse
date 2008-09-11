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

#include "Grammar.h"
#include "Sentence.h"
#include "Condition.h"
#include "Action.h"
#include "Params.h"
#include "Random.h"
#include "CYK.h"
#include "GCS.h"

//TODO temporary
#include <QtDebug>

void Grammar::initGrammar(int nonterminals, int rules)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	mFitness = 0.0;
	mNumberOfSentences = 0;
	mParsedPositive = 0;
	mNotParsedNegative = 0;
	N.clear();
	N << Start;
	NSymbol::resetGenerator();
	for (int i = 0; i < nonterminals; i++)
	{
		N << NSymbol::generateNew();
	}
	PN.clear();
	int Nsize = N.size();
	if (!N.empty())
	{
		for (int i = 0; i < rules; i++)
		{
			NSymbol s1, s2, s3;
			s1 = N[Random::rand(Nsize)];
			s2 = N[Random::rand(Nsize)];
			s3 = N[Random::rand(Nsize)];
			NCondition cond(s1, s2);
			Action act(s3);
			NClassifier cl(cond, act);
			if (!PN.contains(cl))
			{
				PN << cl;
			}
		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
}

void Grammar::induct(const QList<Sentence>& sentences, GCS& parent)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	mNumberOfSentences = sentences.size();
	mNotParsedNegative = 0;
	mParsedPositive = 0;
	if (mNumberOfSentences == 0)
	{
		return;
	}
	resetClParams();
//	Grammar operatingGrammar(*this);
//	if (Params::instance().allowCorrection)
//	{
//		operatingGrammar.correction();
//	}
	int nextEmit = 20;
	parent.nextSentence(0);
	for (int i = 0; i < mNumberOfSentences; i++)
	{
		bool result = CYK::parse(sentences[i], *this);
		if (result == true && sentences[i].isPositive)
		{
			mParsedPositive++;
		}
		else if (result == false && !sentences[i].isPositive)
		{
			mNotParsedNegative++;
		}
		if (!Params::instance().learningMode && i == nextEmit)
		{
			parent.nextSentence(i);
			nextEmit += 20;
		}
	}
	parent.nextSentence(mNumberOfSentences);
//	copyClParameters(operatingGrammar);
	if (Params::instance().learningMode)
	{
		computeMaxClPointsDifference();
		computeMinClPointsDifference();
		for (int i = 0, size = PN.size(); i < size; i++)
		{
			PN[i].computeFitness(mMinClPointsDifference, mMaxClPointsDifference);
		}
		for (int i = 0, size = PT.size(); i < size; i++)
		{
			PT[i].computeFitness(mMinClPointsDifference, mMaxClPointsDifference);
		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
}

void Grammar::resetClParams()
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	for (int i = 0, size = PN.size(); i < size; i++)
	{
		PN[i].resetParams();
	}
	for (int i = 0, size = PT.size(); i < size; i++)
	{
		PT[i].resetParams();
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
}

void Grammar::correction()
{
	//TODO brak implementacji
}

//void Grammar::copyClParameters(const Grammar& other)
//{
////	qDebug() << QString() + __FUNCTION__ + " start";
//	for (int i = 0, size = other.PN.size(); i < size; i++)
//	{
//		int dest = PN.indexOf(other.PN[i]);
//		if (dest != -1)
//		{
//			PN[dest] = other.PN[i];
//		}
//		else
//		{
//			PN << other.PN[i];
//		}
//	}
//	for (int i = 0, size = other.PT.size(); i < size; i++)
//	{
//		int dest = PT.indexOf(other.PT[i]);
//		if (dest != -1)
//		{
//			PT[dest] = other.PT[i];
//		}
//		else
//		{
//			PT << other.PT[i];
//		}
//	}
////	qDebug() << QString() + __FUNCTION__ + " end";
//}

NClassifier* Grammar::addClWithCrowding(const NClassifier& newCl, QList<NClassifier>& set, int maxSize)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	Params& p = Params::instance();
	int setSize = set.size();
	if (set.isEmpty() || (setSize < maxSize && !set.contains(newCl)))
	{
		set << newCl;
//		qDebug() << QString() + __FUNCTION__ + " end";
		return &set.last();
	}
	int idx;
	if ((idx = set.indexOf(newCl)) != -1)
	{
		return &set[idx];
	}
	QList<NClassifier*> K;
	for (int i = 0; i < p.crowdFactor; i++)
	{
		NClassifier* worst = &set[Random::rand(setSize)];
		for (int c = 1; c < p.crowdSize; c++)
		{
			NClassifier* temp = &set[Random::rand(setSize)];
			if (temp->fitness < worst->fitness)
			{
				worst = temp;
			}
		}
		K << worst;
	}

	NClassifier* mostSimilar = *(K.begin());
	for (int i = 0, size = K.size(); i < size; i++)
	{
		if (K[i]->howSimilar(newCl) > mostSimilar->howSimilar(newCl))
		{
			mostSimilar = K[i];
		}
	}
	*mostSimilar = newCl;
//	qDebug() << QString() + __FUNCTION__ + " end";
	return mostSimilar;
}

TClassifier* Grammar::addClWithCrowding(const TClassifier& newCl, QList<TClassifier>& set, int maxSize)
{
//	qDebug() << QString() + __FUNCTION__ + " start";
//	Params& p = Params::instance();
	int setSize = set.size();
	if (set.isEmpty() || (setSize < maxSize && !set.contains(newCl)))
	{
		set << newCl;
//		qDebug() << QString() + __FUNCTION__ + " end";
		return &set.last();
	}
	int idx;
	if ((idx = set.indexOf(newCl)) != -1)
	{
		return &set[idx];
	}
////////////////////////

	for (int i = 0; i < setSize; i++)
	{
		if (newCl.condition == set[i].condition)
		{
			set[i] = newCl;
			return &set[i];
		}
	}
	return NULL;
////////////////////////
//	QSet<TClassifier*> K;
//	for (int i = 0; i < p.crowdFactor; i++)
//	{
//		TClassifier* worst = &set[Random::rand(setSize)];
//		for (int c = 1; c < p.crowdSize; c++)
//		{
//			TClassifier* temp = &set[Random::rand(setSize)];
//			if (temp->fitness < worst->fitness)
//			{
//				worst = temp;
//			}
//		}
//		K << worst;
//	}
//
//	TClassifier* mostSimilar = *(K.begin());
//	QSet<TClassifier*>::iterator iter = K.begin();
//	for (iter++; iter != K.end(); iter++)
//	{
//		if ((*iter)->howSimilar(newCl) > mostSimilar->howSimilar(newCl))
//		{
//			mostSimilar = *iter;
//		}
//	}
//	*mostSimilar = newCl;
//	qDebug() << QString() + __FUNCTION__ + " end";
//	return mostSimilar;
}

int Grammar::computeMaxClPointsDifference()
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	if (PN.isEmpty())
	{
		mMaxClPointsDifference = 0;
	}
	else
	{
		mMaxClPointsDifference = PN[0].pointsDifference();
		for (int i = 1, size = PN.size(); i < size; i++)
		{
			if (PN[i].pointsDifference() > mMaxClPointsDifference)
			{
				mMaxClPointsDifference = PN[i].pointsDifference();
			}
		}
//		for (int i = 0, size = PT.size(); i < size; i++)
//		{
//			if (PT[i].pointsDifference() > max)
//			{
//				max = PT[i].pointsDifference();
//			}
//		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return mMaxClPointsDifference;
}

int Grammar::computeMinClPointsDifference()
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	if (PN.isEmpty())
	{
		mMinClPointsDifference = 0;
	}
	else
	{
		mMinClPointsDifference = PN[0].pointsDifference();
		for (int i = 1, size = PN.size(); i < size; i++)
		{
			if (PN[i].pointsDifference() < mMinClPointsDifference)
			{
				mMinClPointsDifference = PN[i].pointsDifference();
			}
		}
//		for (int i = 0, size = PT.size(); i < size; i++)
//		{
//			if (PT[i].pointsDifference() < min)
//			{
//				min = PT[i].pointsDifference();
//			}
//		}
	}
//	qDebug() << QString() + __FUNCTION__ + " end";
	return mMinClPointsDifference;
}

QString Grammar::toString() const
{
	QString out;
	out += "Produkcje terminalne:\n";
	for (int i = 0, size = PT.size(); i < size; i++)
	{
		out += PT[i].toString() + ", ";
	}
	out += "\nProdukcje nieterminalne:\n";
	for (int i = 0, size = PN.size(); i < size; i++)
	{
		out += PN[i].toString() + ", ";
	}
	out += "\n";
	return out;
}
