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

#include "classifier.h"

///////////////////////////////////////////////////
//class Classifier (only for inheritence)
///////////////////////////////////////////////////

int Classifier::mffmax = 0;
int Classifier::mffmin = 0;

Classifier::Classifier()
{
    this->resetParams();
}

float Classifier::fitness() const
{
    return this->mFitness;
}

float Classifier::computeFitness()
{
    float classicFun = 0.0;
    if (this->mup > 0 || this->mun > 0)//classifier used at least once
    {
        classicFun = (float)(Params::positiveSentenceWeight() * this->mup) / (Params::negativeSentenceWeight() * this->mun + Params::positiveSentenceWeight() * this->mup);
    }
    else//classifier not used
    {
        classicFun = Params::unusedClassifierFitness();
    }

    float fertilityFun = 0.0;
    fertilityFun = (float)(this->mp - this->md - mffmin)/(mffmax - mffmin);

    this->mFitness = (Params::classicFunWeight() * classicFun + Params::fertilityFunWeight() * fertilityFun)/(Params::classicFunWeight() + Params::fertilityFunWeight());
    return this->mFitness;
}

void Classifier::setFitness(float f)
{
    this->mFitness = f;
}

void Classifier::resetParams()
{
    this->mFitness = 0.0;
    this->mup = 0;
    this->mun = 0;
    this->mp = 0;
    this->md = 0;
}

bool Classifier::operator <(const Classifier& other) const
{
    return this->fitness() < other.fitness();
}

///////////////////////////////////////////////////
//class NClassifier
///////////////////////////////////////////////////

NClassifier::NClassifier()
{
}

NClassifier::NClassifier(const NProdRule& prod) :
    NProdRule(prod)
{
}

const NProdAction& NClassifier::condition() const
{
    return NProdRule::action();
}

void NClassifier::setCondition(const NProdAction& cond)
{
    NProdRule::setAction(cond);
}

const ProdCondition& NClassifier::action() const
{
    return NProdRule::condition();
}

void NClassifier::setAction(const ProdCondition& act)
{
    NProdRule::setCondition(act);
}

const ProdCondition& NClassifier::prodCondition() const
{
    return NProdRule::condition();
}

void NClassifier::setProdCondition(const ProdCondition& act)
{
    NProdRule::setCondition(act);
}

const NProdAction& NClassifier::prodAction() const
{
    return NProdRule::action();
}

void NClassifier::setProdAction(const NProdAction& cond)
{
    NProdRule::setAction(cond);
}

NClassifier::operator QString() const
{
    return NProdRule::operator QString() + "/" + QString::number(this->fitness());
}

int NClassifier::similar(const NClassifier& other) const
{
    int c = 0;
    if(this->prodCondition() == other.prodCondition())
        c++;
    if(this->prodAction().firstSymbol() == other.prodAction().firstSymbol())
        c++;
    if(this->prodAction().secondSymbol() == other.prodAction().secondSymbol())
        c++;
    return c;
}

///////////////////////////////////////////////////
//class TClassifier
///////////////////////////////////////////////////

TClassifier::TClassifier()
{
}

TClassifier::TClassifier(const TProdRule& prod) :
    TProdRule(prod)
{
}

const TProdAction& TClassifier::condition() const
{
    return TProdRule::action();
}

void TClassifier::setCondition(const TProdAction& cond)
{
    TProdRule::setAction(cond);
}

const ProdCondition& TClassifier::action() const
{
    return TProdRule::condition();
}

void TClassifier::setAction(const ProdCondition& act)
{
    TProdRule::setCondition(act);
}

const ProdCondition& TClassifier::prodCondition() const
{
    return TProdRule::condition();
}

void TClassifier::setProdCondition(const ProdCondition& act)
{
    TProdRule::setCondition(act);
}

const TProdAction& TClassifier::prodAction() const
{
    return TProdRule::action();
}

void TClassifier::setProdAction(const TProdAction& cond)
{
    TProdRule::setAction(cond);
}

TClassifier::operator QString() const
{
    return TProdRule::operator QString() + "/" + QString::number(this->fitness());
}

int TClassifier::similar(const TClassifier& other) const
{
    int c = 0;
    if(this->prodCondition() == other.prodCondition())
        c++;
    if(this->prodAction().symbol() == other.prodAction().symbol())
        c++;
    return c;
}
