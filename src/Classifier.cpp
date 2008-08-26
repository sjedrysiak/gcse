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

	fitness = (p.classicFunWeight * classicFun + p.fertilityFunWeight * fertilityFun) / (p.classicFunWeight + p.fertilityFunWeight);
	return fitness;
}

///////////////////////////////////////////////////
//class NClassifier
///////////////////////////////////////////////////

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

///////////////////////////////////////////////////
//class TClassifier
///////////////////////////////////////////////////

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

void TClassifier::resetParams()
{
	fitness = 0.0;
	mup = 0;
	mun = 0;
	mp = Params::instance().baseAmount;
	md = Params::instance().baseAmount;
}
