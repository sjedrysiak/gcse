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
#include "Random.h"
#include "Params.h"
#include "CYK.h"
#include "Sentence.h"
#include <QStringList>

Grammar::Grammar(const NSymbol& start, const NSymbol& universal) :
	S(start), Su(universal)
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
	this->N << this->S;
	this->N << this->Su;
	NSymbol A, B, C, D;
	TSymbol a("a"), b("b"), c("c");
	this->N << A;
	this->N << B;
	this->N << C;
	this->N << D;
	this->T << a;
	this->T << b;
	this->T << c;

	this->PN << NClassifier(NProdRule(Condition(this->S), NProdAction(A, B))); // S -> AB
	this->PN << NClassifier(NProdRule(Condition(this->S), NProdAction(A, C))); // S -> AC
	this->PN << NClassifier(NProdRule(Condition(C), NProdAction(this->S, B))); // C -> SB
	this->PN << NClassifier(NProdRule(Condition(B), NProdAction(B, B))); // B -> BB
	this->PT << TClassifier(TProdRule(Condition(C), TProdAction(a))); // C -> a
	this->PT << TClassifier(TProdRule(Condition(B), TProdAction(b))); // B -> b
	this->PT << TClassifier(TProdRule(Condition(A), TProdAction(a))); // A -> a
}

void Grammar::induct(const QList<Sentence>& sentences)
{
	this->mNumberOfSentences = sentences.size();
	this->initClParams();
	Grammar operatingGrammar(*this);
	if (Params::allowCorrection())
	{
		operatingGrammar.correction();
	}
	foreach (Sentence sentence, sentences){
	bool result = CYK::parse(sentence, operatingGrammar);
	if (result == true && sentence.isPositive())
	{
		this->mParsedPositive++;
	}
	else if (result == false && !sentence.isPositive())
	{
		this->mNotParsedNegative++;
	}
	//TODO aktualizacja parametrów produkcji (przenieść do parse())
}
this->copyClParameters(operatingGrammar);
foreach (NClassifier cl, this->PN)
{
	cl.computeFitness(*this);
}
}

void Grammar::initClParams()
{
	foreach (NClassifier cl, this->PN){
	cl.resetParams();
}
foreach (TClassifier cl, this->PT)
{
	cl.resetParams();
}
}

void Grammar::correction()
{
}

float Grammar::computeFitness()
{
	this->mFitness = (float) (this->mParsedPositive + this->mNotParsedNegative) / this->mNumberOfSentences;
	return this->mFitness;
}

float Grammar::fitness() const
{
	return this->mFitness;
}

void Grammar::copyClParameters(const Grammar& other)
{
	for (int i = 0; i < this->PN.size(); i++)// QList<NClassifier>::iterator dest = this->PN.begin(); dest != this->PN.end(); ++dest)
	{
		int src = other.PNSet().indexOf(this->PN[i]);
		if (src != -1)
		{
			this->PN[i] = other.PN[src];
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
void Grammar::addSymbol(const NSymbol &s)
{
	this->N << s;
}

void Grammar::addSymbol(const TSymbol &s)
{
	this->T << s;
}

void Grammar::addClNormal(const NClassifier& cl)
{
	this->addSymbol(cl.prodCondition().symbol());
	this->addSymbol(cl.prodAction().firstSymbol());
	this->addSymbol(cl.prodAction().secondSymbol());
	this->PN << cl;
}

void Grammar::addClNormal(const TClassifier& cl)
{
	this->addSymbol(cl.prodCondition().symbol());
	this->addSymbol(cl.prodAction().symbol());
	this->PT << cl;
}

void Grammar::addClWithCrowding(const NClassifier& newCl, QList<NClassifier>& set)
{
	if (set.contains(newCl))
	{
		return;
	}

	QSet<NClassifier> K;
	for (int i = 0; i < Params::crowdFactor(); i++)
	{
		NClassifier worst = set[Random::rand(set.size())];
		QSet<NClassifier> W;
		W << worst;
		while (W.size() < Params::crowdSize())
		{
			NClassifier temp = set[Random::rand(set.size())];
			W << temp;
			if (temp.fitness() < worst.fitness())
			{
				worst = temp;
			}
		}
		K << worst;
	}

	QList<NClassifier> list = K.toList();
	int mostSimilar = 0;
	for (int i = 1; i < list.size(); i++)
	{
		if (list[i].howSimilar(newCl) > list[mostSimilar].howSimilar(newCl))
		{
			mostSimilar = i;
		}
	}
	set[mostSimilar] = newCl;
}

void Grammar::addClWithCrowding(const TClassifier& newCl, QList<TClassifier>& set)
{
	if (set.contains(newCl))
	{
		return;
	}

	QList<TClassifier> classifierList(set);

	QSet<TClassifier> K;
	for (int i = 0; i < Params::crowdFactor(); i++)
	{
		TClassifier worst = classifierList[Random::rand(classifierList.size())];
		QSet<TClassifier> W;
		W << worst;
		while (W.size() < Params::crowdSize())
		{
			TClassifier temp = classifierList[Random::rand(classifierList.size())];
			W << temp;
			if (temp.fitness() < worst.fitness())
			{
				worst = temp;
			}
		}
		K << worst;
	}

	QList<TClassifier> list = K.toList();
	int mostSimilar = 0;
	for (int i = 1; i < list.size(); i++)
	{
		if (list[i].howSimilar(newCl) > list[mostSimilar].howSimilar(newCl))
		{
			mostSimilar = i;
		}
	}
	set[mostSimilar] = newCl;
}

int Grammar::maxClPointsDifference() const
{
	return this->mMaxClPointsDifference;
}

int Grammar::computeMaxClPointsDifference()
{
	return this->mMaxClPointsDifference = 0;
}

int Grammar::minClPointsDifference() const
{
	return this->mMinClPointsDifference;
}

int Grammar::computeMinClPointsDifference()
{
	return this->mMinClPointsDifference = 0;
}

Grammar::~Grammar()
{
}
