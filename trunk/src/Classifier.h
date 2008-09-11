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

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "Condition.h"
#include "Action.h"

class Classifier
{
public:
	float computeFitness(int minDifference, int maxDifference);
	virtual void resetParams()
	{
		fitness = 0.0;
		mup = 0;
		mun = 0;
		mp = 0;
		md = 0;
	}
	bool operator <(const Classifier& other) const
	{
		return fitness < other.fitness;
	}
	int pointsDifference() const
	{
		return mp - md;
	}
	void used(bool positiveSentence)
	{
		positiveSentence ? mup++ : mun++;
	}
	void increasePoints(bool positiveSentence, int points)
	{
		positiveSentence ? mp += points : md += points;
	}
	virtual QString toString() const = 0;

	Action action;
	/**
	 * classifier fitness
	 */
	float fitness;
protected:
	Classifier(const Action& act) :
		action(act)
	{
		resetParams();
	}
	/**
	 * used in positive sentence
	 */
	unsigned int mup;
	/**
	 * used in negative sentence
	 */
	unsigned int mun;
	/**
	 * points for positive sentences
	 */
	unsigned int mp;
	/**
	 * points for negative sentences
	 */
	unsigned int md;
};

class NClassifier: public Classifier
{
public:
	NClassifier(const NCondition& cond, const Action& act) :
		Classifier(act), condition(cond)
	{
	}
	int howSimilar(const NClassifier& other) const;
	virtual QString toString() const
	{
		return action.toString() + "=>" + condition.toString() + " (" + QString::number(fitness) + ")";
	}
	bool operator ==(const NClassifier& other) const
	{
		return condition == other.condition && action == other.action;
	}

	NCondition condition;
};

class TClassifier: public Classifier
{
public:
	TClassifier(const TCondition& cond, const Action& act) :
		Classifier(act), condition(cond)
	{
		resetParams();
	}
	int howSimilar(const TClassifier& other) const;
	virtual QString toString() const
	{
		return action.toString() + "=>" + condition.toString() + " (" + QString::number(fitness) + ")";
	}
	bool operator ==(const TClassifier& other) const
	{
		return condition == other.condition && action == other.action;
	}
	void resetParams();

	TCondition condition;
};

#endif /*CLASSIFIER_H_*/
