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

#include "Classifier.h"
#include "Params.h"

///////////////////////////////////////////////////
//class Classifier (only for inheritence)
///////////////////////////////////////////////////

Classifier::Classifier(const Action& act) :
	action(act)
{
	resetParams();
}

float Classifier::fitness() const
{
	return mFitness;
}

float Classifier::computeFitness(int minDifference, int maxDifference)
{
	Params& p = Params::instance();
	float classicFun = 0.0;
	if (mup > 0 || mun > 0)//classifier used at least once
	{
		classicFun = (float) (p.positiveSentenceWeight * mup) / (p.negativeSentenceWeight * mun + p.positiveSentenceWeight * mup);
	}
	else//classifier not used
	{
		classicFun = p.unusedClassifierFitness;
	}

	float fertilityFun = 0.0;
	if (minDifference != maxDifference)
	{
		fertilityFun = (float) (mp - md - minDifference) / (maxDifference - minDifference);
	}

	mFitness = (p.classicFunWeight * classicFun + p.fertilityFunWeight * fertilityFun) / (p.classicFunWeight + p.fertilityFunWeight);
	return mFitness;
}

void Classifier::resetParams()
{
	mFitness = 0.0;
	mup = 0;
	mun = 0;
	mp = 0;
	md = 0;
}

bool Classifier::operator <(const Classifier& other) const
{
	return mFitness < other.mFitness;
}

int Classifier::pointsDifference() const
{
	return mp - md;
}

void Classifier::used(bool positiveSentence)
{
	if (positiveSentence)
	{
		mup++;
	}
	else
	{
		mun++;
	}
}

void Classifier::increasePoints(bool positive, int points)
{
	if (positive)
	{
		mp += points;
	}
	else
	{
		md += points;
	}
}

///////////////////////////////////////////////////
//class NClassifier
///////////////////////////////////////////////////

NClassifier::NClassifier(const NCondition& cond, const Action& act) :
	Classifier(act), condition(cond)
{
}

int NClassifier::howSimilar(const NClassifier& other) const
{
	int c = 0;
	if (condition.firstSymbol == other.condition.firstSymbol)
	{
		c++;
	}
	if (condition.secondSymbol == other.condition.secondSymbol)
	{
		c++;
	}
	if (action == other.action)
	{
		c++;
	}
	return c;
}

QString NClassifier::toString() const
{
	return action.toString() + "=>" + condition.toString() + " (" + QString::number(mFitness) + ")";
}

bool NClassifier::operator ==(const NClassifier& other) const
{
	return condition == other.condition && action == other.action;
}

///////////////////////////////////////////////////
//class TClassifier
///////////////////////////////////////////////////

TClassifier::TClassifier(const TCondition& cond, const Action& act) :
	Classifier(act), condition(cond)
{
	mp = Params::instance().baseAmount;
	md = Params::instance().baseAmount;
}

int TClassifier::howSimilar(const TClassifier& other) const
{
	int c = 0;
	if (condition == other.condition)
	{
		c++;
	}
	if (action == other.action)
	{
		c++;
	}
	return c;
}

QString TClassifier::toString() const
{
	return action.toString() + "=>" + condition.toString() + " (" + QString::number(mFitness) + ")";
}

bool TClassifier::operator ==(const TClassifier& other) const
{
	return condition == other.condition && action == other.action;
}

void TClassifier::resetParams()
{
	mFitness = 0.0;
	mup = 0;
	mun = 0;
	mp = Params::instance().baseAmount;
	md = Params::instance().baseAmount;
}
