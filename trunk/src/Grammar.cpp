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
#include <QSet>

//TODO temporary
#include <QtCore>

Grammar::Grammar(const NSymbol& start, const NSymbol& universal) :
	Start(start), Universal(universal)
{
	this->initGrammar();
}

void Grammar::initGrammar()
{
	this->mFitness = 0.0;
	this->mNumberOfSentences = 0;
	this->mParsedPositive = 0;
	this->mNotParsedNegative = 0;
	this->mMinClPointsDifference = 0;
	this->mMaxClPointsDifference = 0;
//	this->N << this->Start;
//	this->N << this->Universal;
	NSymbol A, B, C, D;
	TSymbol a("a"), b("b"), c("c");
	this->N << A;
	this->N << B;
	this->N << C;
	this->N << D;
	this->T << a;
	this->T << b;
	this->T << c;

	this->PN << NClassifier(NCondition(A, B), Action(this->Start)); // S -> AB
	this->PN << NClassifier(NCondition(A, C), Action(this->Start)); // S -> AC
	this->PN << NClassifier(NCondition(this->Start, B), Action(C)); // C -> SB
	this->PN << NClassifier(NCondition(B, B), Action(B)); // B -> BB
	this->PT << TClassifier(TCondition(a), Action(A)); // A -> a
	this->PT << TClassifier(TCondition(b), Action(B)); // B -> b
	this->PT << TClassifier(TCondition(c), Action(C)); // C -> c
}

void Grammar::induct(const QList<Sentence>& sentences)
{
	this->mNumberOfSentences = sentences.size();
	this->resetClParams();
	Grammar operatingGrammar(*this);
	if (Params::allowCorrection())
	{
		operatingGrammar.correction();
	}
	for (int i = 0; i < this->mNumberOfSentences; i++)
	{
		bool result = CYK::parse(sentences[i], operatingGrammar);
		if (result == true && sentences[i].isPositive())
		{
			this->mParsedPositive++;
		}
		else if (result == false && !sentences[i].isPositive())
		{
			this->mNotParsedNegative++;
		}
	}
	this->copyClParameters(operatingGrammar);
	for (int i = 0; i < this->PN.size(); i++)
	{
		this->PN[i].computeFitness(*this);
	}
	for (int i = 0; i < this->PT.size(); i++)
	{
		this->PT[i].computeFitness(*this);
	}
}

void Grammar::resetClParams()
{
	for (int i = 0; i < this->PN.size(); i++)
	{
		this->PN[i].resetParams();
	}
	for (int i = 0; i < this->PT.size(); i++)
	{
		this->PT[i].resetParams();
	}
}

void Grammar::correction()
{
	//TODO brak implementacji
}

float Grammar::computeFitness()
{
	if (this->mNumberOfSentences > 0)
	{
		this->mFitness = (float) (this->mParsedPositive + this->mNotParsedNegative) / this->mNumberOfSentences;
	}
	return this->mFitness;
}

float Grammar::fitness() const
{
	return this->mFitness;
}

void Grammar::copyClParameters(const Grammar& other)
{
	for (int i = 0; i < this->PN.size(); i++)
	{
		int src = other.PN.indexOf(this->PN[i]);
		if (src != -1)
		{
			this->PN[i] = other.PN[src];
		}
	}
	for (int i = 0; i < this->PT.size(); i++)
	{
		int src = other.PT.indexOf(this->PT[i]);
		if (src != -1)
		{
			this->PT[i] = other.PT[src];
		}
	}
}

const QList<NSymbol>& Grammar::NSet() const
{
	return this->N;
}

const QList<TSymbol>& Grammar::TSet() const
{
	return this->T;
}

const QList<NClassifier>& Grammar::PNSet() const
{
	return this->PN;
}

const QList<TClassifier>& Grammar::PTSet() const
{
	return this->PT;
}

void Grammar::setN(const QList<NSymbol>& source)
{
	this->N = source;
}

void Grammar::setT(const QList<TSymbol>& source)
{
	this->T = source;
}

void Grammar::setPN(const QList<NClassifier>& source)
{
	this->PN = source;
}

void Grammar::setPT(const QList<TClassifier>& source)
{
	this->PT = source;
}

//adding methods
void Grammar::addSymbol(const NSymbol& s)
{
	if (!this->N.contains(s))
	{
		this->N << s;
	}
}

void Grammar::addSymbol(const TSymbol& s)
{
	if (!this->T.contains(s))
	{
		this->T << s;
	}
}

void Grammar::addClNormal(const NClassifier& cl)
{
	if (!this->PN.contains(cl))
	{
		this->addSymbol(cl.condition().firstSymbol());
		this->addSymbol(cl.condition().secondSymbol());
		this->addSymbol(cl.action().symbol());
		this->PN << cl;
	}
}

void Grammar::addClNormal(const TClassifier& cl)
{
	if (!this->PT.contains(cl))
	{
		this->addSymbol(cl.condition().symbol());
		this->addSymbol(cl.action().symbol());
		this->PT << cl;
	}
}

void Grammar::addClWithCrowding(const NClassifier& newCl, QList<NClassifier>& set)
{
//	if (set.contains(newCl))
//	{
//		return;
//	}

	QList<NClassifier*> K;
	for (int i = 0; i < Params::crowdFactor(); i++)
	{
		NClassifier* worst = &set[Random::rand(set.size())];
		QSet<NClassifier*> W;
		W << worst;
		while (W.size() < Params::crowdSize())
		{
			NClassifier* temp = &set[Random::rand(set.size())];
			W << temp;
			if (temp->fitness() < worst->fitness())
			{
				worst = temp;
			}
		}
		K << worst;
	}

	int mostSimilar = 0;
	for (int i = 1; i < K.size(); i++)
	{
		if (K[i]->howSimilar(newCl) > K[mostSimilar]->howSimilar(newCl))
		{
			mostSimilar = i;
		}
	}
	*(K[mostSimilar]) = newCl;
}

void Grammar::addClWithCrowding(const TClassifier& newCl, QList<TClassifier>& set)
{
//	if (set.contains(newCl))
//	{
//		return;
//	}

	QList<TClassifier*> K;
	for (int i = 0; i < Params::crowdFactor(); i++)
	{
		TClassifier* worst = &set[Random::rand(set.size())];
		QSet<TClassifier*> W;
		W << worst;
		while (W.size() < Params::crowdSize())
		{
			TClassifier* temp = &set[Random::rand(set.size())];
			W << temp;
			if (temp->fitness() < worst->fitness())
			{
				worst = temp;
			}
		}
		K << worst;
	}

	int mostSimilar = 0;
	for (int i = 1; i < K.size(); i++)
	{
		if (K[i]->howSimilar(newCl) > K[mostSimilar]->howSimilar(newCl))
		{
			mostSimilar = i;
		}
	}
	*K[mostSimilar] = newCl;
}

int Grammar::maxClPointsDifference() const
{
	return this->mMaxClPointsDifference;
}

int Grammar::computeMaxClPointsDifference()
{
	if (this->PN.size() == 0)
	{
		this->mMaxClPointsDifference = 0;
	}
	else
	{
		this->mMaxClPointsDifference = this->PN[0].pointsDifference();
		for (int i = 1; i < this->PN.size(); i++)
		{
			if (this->PN[i].pointsDifference() > this->mMaxClPointsDifference)
			{
				this->mMaxClPointsDifference = this->PN[i].pointsDifference();
			}
		}
		for (int i = 0; i < this->PT.size(); i++)
		{
			if (this->PT[i].pointsDifference() > this->mMaxClPointsDifference)
			{
				this->mMaxClPointsDifference = this->PT[i].pointsDifference();
			}
		}
	}
	return this->mMaxClPointsDifference;
}

int Grammar::minClPointsDifference() const
{
	return this->mMinClPointsDifference;
}

int Grammar::computeMinClPointsDifference()
{
	if (this->PN.size() == 0)
	{
		this->mMinClPointsDifference = 0;
	}
	else
	{
		this->mMinClPointsDifference = this->PN[0].pointsDifference();
		for (int i = 1; i < this->PN.size(); i++)
		{
			if (this->PN[i].pointsDifference() < this->mMinClPointsDifference)
			{
				this->mMinClPointsDifference = this->PN[i].pointsDifference();
			}
		}
		for (int i = 0; i < this->PT.size(); i++)
		{
			if (this->PT[i].pointsDifference() < this->mMinClPointsDifference)
			{
				this->mMinClPointsDifference = this->PT[i].pointsDifference();
			}
		}
	}
	return this->mMinClPointsDifference;
}

Grammar::~Grammar()
{
}
