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
	float fitness() const;
	float computeFitness(int minDifference, int maxDifference);
	virtual void resetParams();
	bool operator <(const Classifier& other) const;
	int pointsDifference() const;
	void used(bool positiveSentence);
	void increasePoints(bool positive, int points);

	Action action;
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
};

class NClassifier: public Classifier
{
public:
	NClassifier(const NCondition& cond, const Action& act);
	int howSimilar(const NClassifier& other) const;
	QString toString() const;
	bool operator ==(const NClassifier& other) const;

	NCondition condition;
};

class TClassifier: public Classifier
{
public:
	TClassifier(const TCondition& cond, const Action& act);
	int howSimilar(const TClassifier& other) const;
	QString toString() const;
	bool operator ==(const TClassifier& other) const;
	void resetParams();

	TCondition condition;
};

#endif /*CLASSIFIER_H_*/
