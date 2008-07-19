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
class Grammar;

class Classifier
{
public:
	float fitness() const;
	float computeFitness(/*const Grammar& g*/);
	void resetParams();
	bool operator <(const Classifier& other) const;
	int pointsDifference() const;
	void used(bool positiveSentence);
	void increasePoints(bool positive, int points);
	const Action& action() const;
	void setAction(const Action& act);
protected:
	Classifier(const Action& act);
	/**
	 * classifier fitness
	 */
	float mFitness;
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
	Action mAction;
};

class NClassifier: public Classifier
{
public:
	NClassifier(const NCondition& cond, const Action& act);
	const NCondition& condition() const;
	void setCondition(const NCondition& cond);
	int howSimilar(const NClassifier& other) const;
	operator QString() const;
	bool operator ==(const NClassifier& other) const;
protected:
	NCondition mCondition;
};

class TClassifier: public Classifier
{
public:
	TClassifier(const TCondition& cond, const Action& act);
	const TCondition& condition() const;
	void setCondition(const TCondition& cond);
	int howSimilar(const TClassifier& other) const;
	operator QString() const;
	bool operator ==(const TClassifier& other) const;
protected:
	TCondition mCondition;
};

#endif /*CLASSIFIER_H_*/
