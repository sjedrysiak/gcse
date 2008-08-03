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
#include "Grammar.h"

///////////////////////////////////////////////////
//class Classifier (only for inheritence)
///////////////////////////////////////////////////

Classifier::Classifier(const Action& act) :
	mAction(act)
{
	this->resetParams();
}

float Classifier::fitness() const
{
	return this->mFitness;
}

float Classifier::computeFitness(const Grammar& g)
{
	float classicFun = 0.0;
	if (this->mup > 0 || this->mun > 0)//classifier used at least once
	{
		classicFun = (float) (Params::positiveSentenceWeight() * this->mup) / (Params::negativeSentenceWeight() * this->mun + Params::positiveSentenceWeight() * this->mup);
	}
	else//classifier not used
	{
		classicFun = Params::unusedClassifierFitness();
	}

	float fertilityFun = 0.0;
	fertilityFun = (float) (this->mp - this->md - g.minClPointsDifference()) / (g.maxClPointsDifference() - g.minClPointsDifference());

	this->mFitness = (Params::classicFunWeight() * classicFun + Params::fertilityFunWeight() * fertilityFun) / (Params::classicFunWeight() + Params::fertilityFunWeight());
	return this->mFitness;
}

void Classifier::resetParams()
{
	this->mFitness = 0.0;
	this->mup = 0;
	this->mun = 0;
	this->mp = Params::baseAmount();//TODO czy to może być tutaj?
	this->md = Params::baseAmount();
}

bool Classifier::operator <(const Classifier& other) const
{
	return this->fitness() < other.fitness();
}

int Classifier::pointsDifference() const
{
	return this->mp - this->md;
}

void Classifier::used(bool positiveSentence)
{
	if (positiveSentence)
	{
		this->mup++;
	}
	else
	{
		this->mun++;
	}
}

void Classifier::increasePoints(bool positive, int points)
{
	if (positive)
	{
		this->mp += points;
	}
	else
	{
		this->md += points;
	}
}

const Action& Classifier::action() const
{
	return this->mAction;
}

void Classifier::setAction(const Action& act)
{
	this->mAction = act;
}

///////////////////////////////////////////////////
//class NClassifier
///////////////////////////////////////////////////

NClassifier::NClassifier(const NCondition& cond, const Action& act) :
	Classifier(act), mCondition(cond)
{
}

const NCondition& NClassifier::condition() const
{
	return this->mCondition;
}

void NClassifier::setCondition(const NCondition& cond)
{
	this->mCondition = cond;
}

int NClassifier::howSimilar(const NClassifier& other) const
{
	int c = 0;
	if (this->mCondition.firstSymbol() == other.mCondition.firstSymbol())
	{
		c++;
	}
	if (this->mCondition.secondSymbol() == other.mCondition.secondSymbol())
	{
		c++;
	}
	if (this->mAction == other.mAction)
	{
		c++;
	}
	return c;
}

NClassifier::operator QString() const
{
	return this->mCondition + "<-" + this->mAction + "/" + QString::number(this->mFitness);
}

bool NClassifier::operator ==(const NClassifier& other) const
{
	return this->mCondition == other.mCondition && this->mAction == other.mAction;
}

///////////////////////////////////////////////////
//class TClassifier
///////////////////////////////////////////////////

TClassifier::TClassifier(const TCondition& cond, const Action& act) :
	Classifier(act), mCondition(cond)
{
}

const TCondition& TClassifier::condition() const
{
	return this->mCondition;
}

void TClassifier::setCondition(const TCondition& cond)
{
	this->mCondition = cond;
}

int TClassifier::howSimilar(const TClassifier& other) const
{
	int c = 0;
	if (this->mCondition == other.mCondition)
	{
		c++;
	}
	if (this->mAction == other.mAction)
	{
		c++;
	}
	return c;
}

TClassifier::operator QString() const
{
	return this->mCondition + "<-" + this->mAction + "/" + QString::number(this->mFitness);
}

bool TClassifier::operator ==(const TClassifier& other) const
{
	return this->mCondition == other.mCondition && this->mAction == other.mAction;
}
