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

#include "grammar.h"
#include "random.h"
#include "params.h"
#include "cyk.h"
#include "sentence.h"
#include <QStringList>
#include <QList>

Grammar::Grammar(const NSymbol& start, const NSymbol& universal) :
    S(start), Su(universal)
{
    this->N << this->S;
    this->N << this->Su;
    this->initGrammar();
    this->mFitness = 0.0;
    this->mNumberOfSentences = 0;
    this->mParsedPositive = 0;
    this->mNotParsedNegative = 0;
    this->mMinClPointsDifference = 0;
    this->mMaxClPointsDifference = 0;
}

void Grammar::initGrammar()
{
    NSymbol A("A"), B("B"), C("C"), D("D");
    TSymbol a("a"), b("b"), c("c");
    this->N << A;
    this->N << B;
    this->N << C;
    this->N << D;
    this->T << a;
    this->T << b;
    this->T << c;

    this->PN << NClassifier( NProdRule( ProdCondition(this->S), NProdAction(A,B) ) ); // S -> AB
    this->PN << NClassifier( NProdRule( ProdCondition(this->S), NProdAction(A,C) ) ); // S -> AC
    this->PN << NClassifier( NProdRule( ProdCondition(C), NProdAction(this->S,B) ) ); // C -> SB
    this->PN << NClassifier( NProdRule( ProdCondition(B), NProdAction(B,B) ) ); // B -> BB
    this->PT << TClassifier( TProdRule( ProdCondition(C), TProdAction(a) ) ); // C -> a
    this->PT << TClassifier( TProdRule( ProdCondition(B), TProdAction(b) ) ); // B -> b
    this->PT << TClassifier( TProdRule( ProdCondition(A), TProdAction(a) ) ); // A -> a
}

void Grammar::induct(const QList<Sentence>& sentences)
{
    this->mNumberOfSentences = sentences.size();
    //init parametry produkcji
    Grammar operatingGrammar(*this);
    //copy parametry produkcji do G*
    if (Params::allowCorrection())
    {
        operatingGrammar.correction();
    }
    foreach (Sentence sentence, sentences)
    {
        bool result = CYK::parse(sentence, operatingGrammar);
        if (result == true && sentence.isPositive())
        {
            this->mParsedPositive++;
        }
        else if (result == false && !sentence.isPositive())
        {
            this->mNotParsedNegative++;
        }
        //aktualizacja parametrów produkcji (przenieść do parse())
    }
    //copy parametry produkcji z G* do G
    foreach (NClassifier cl, this->PNSet())
    {
        cl.computeFitness(*this);
    }
}

void Grammar::correction()
{
}

float Grammar::computeFitness()
{
    this->mFitness = (float)(this->mParsedPositive + this->mNotParsedNegative) / this->mNumberOfSentences;
    return this->mFitness;
}

float Grammar::fintess() const
{
    return this->mFitness;
}

const QSet<NSymbol>& Grammar::NSet() const
{
    return this->N;
}

const QSet<TSymbol>& Grammar::TSet() const
{
    return this->T;
}

const QSet<NClassifier>& Grammar::PNSet() const
{
    return this->PN;
}

const QSet<TClassifier>& Grammar::PTSet() const
{
    return this->PT;
}

void Grammar::setN(const QSet<NSymbol>& source)
{
    this->N = source;
}

void Grammar::setT(const QSet<TSymbol>& source)
{
    this->T = source;
}

void Grammar::setPN(const QSet<NClassifier>& source)
{
    this->PN = source;
}

void Grammar::setPT(const QSet<TClassifier>& source)
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

void Grammar::addClWithCrowding(const NClassifier& newCl, QSet<NClassifier>& set)
{
    if (set.contains(newCl))
    {
        return;
    }

    QList<NClassifier> vect(set.toList());

    QSet<NClassifier> K;
    for (int i = 0; i < Params::crowdFactor(); i++)
    {
        NClassifier worst = vect[Random::rand(vect.size())];
        QSet<NClassifier> W;
        W << worst;
        while (W.size() < Params::crowdSize())
        {
            NClassifier temp = vect[Random::rand(vect.size())];
            W << temp;
            if (temp.fitness() < worst.fitness())
            {
                worst = temp;
            }
        }
        K << worst;
    }

    NClassifier mostSimilar = K.toList()[0];
    foreach (NClassifier temp, K)
    {
        if (temp.similar(newCl) > mostSimilar.similar(newCl))
        {
            mostSimilar = temp;
        }
    }
    set.remove(mostSimilar);
    set << newCl;
}

void Grammar::addClWithCrowding(const TClassifier& newCl, QSet<TClassifier>& set)
{
    if (set.contains(newCl))
    {
        return;
    }

    QList<TClassifier> classifierList(set.toList());

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

    TClassifier mostSimilar = K.toList()[0];
    foreach (TClassifier temp, K)
    {
        if (temp.similar(newCl) > mostSimilar.similar(newCl))
        {
            mostSimilar = temp;
        }
    }
    set.remove(mostSimilar);
    set << newCl;
}

int Grammar::maxClPointsDifference() const
{
    return this->mMaxClPointsDifference;
}

void Grammar::setMaxClPointsDifference(int value)
{
    this->mMaxClPointsDifference = value;
}

int Grammar::minClPointsDifference() const
{
    return this->mMinClPointsDifference;
}

void Grammar::setMinClPointsDifference(int value)
{
    this->mMinClPointsDifference = value;
}

Grammar::~Grammar()
{
}
