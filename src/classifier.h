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

#include "prodrule.h"

class Classifier
{
public:
    double fitness() const;
    void setFitness(double f);
    bool operator <(const Classifier& other) const;
protected:
    Classifier();
    //metody do operacji na parametrach...
    double mFitness;
    int mup;
    int mun;
    int mp;
    int md;
};

class NClassifier : public Classifier, public NProdRule
{
public:
    NClassifier();
    NClassifier(const NProdRule& prod);
    const NProdAction& condition() const;
    void setCondition(const NProdAction& cond);
    const ProdCondition& action() const;
    void setAction(const ProdCondition& act);
    const ProdCondition& prodCondition() const;
    void setProdCondition(const ProdCondition& act);
    const NProdAction& prodAction() const;
    void setProdAction(const NProdAction& cond);
    operator QString() const;
    int similar(const NClassifier& other) const;
private:
};

class TClassifier : public Classifier, public TProdRule
{
public:
    TClassifier();
    TClassifier(const TProdRule& prod);
    const TProdAction& condition() const;
    void setCondition(const TProdAction& cond);
    const ProdCondition& action() const;
    void setAction(const ProdCondition& act);
    const ProdCondition& prodCondition() const;
    void setProdCondition(const ProdCondition& act);
    const TProdAction& prodAction() const;
    void setProdAction(const TProdAction& cond);
    operator QString() const;
    int similar(const TClassifier& other) const;
private:
};

#endif /*CLASSIFIER_H_*/
