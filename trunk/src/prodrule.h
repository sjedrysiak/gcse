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

#ifndef PRODRULE_H_
#define PRODRULE_H_

#include "prodcondition.h"
#include "prodaction.h"

class NProdRule
{
public:
    NProdRule();
    NProdRule(const ProdCondition& cond, const NProdAction& act);
    const ProdCondition& condition() const;
    void setCondition(const ProdCondition& cond);
    const NProdAction& action() const;
    void setAction(const NProdAction& act);
    bool operator ==(const NProdRule& other) const;
    operator QString() const;
    ~NProdRule();
protected:
    ProdCondition mCondition;
    NProdAction mAction;
};

class TProdRule
{
public:
    TProdRule();
    TProdRule(const ProdCondition& cond, const TProdAction& act);
    const ProdCondition& condition() const;
    void setCondition(const ProdCondition& cond);
    const TProdAction& action() const;
    void setAction(const TProdAction& act);
    bool operator ==(const TProdRule& other) const;
    operator QString() const;
    ~TProdRule();
protected:
    ProdCondition mCondition;
    TProdAction mAction;
};

#endif /*PRODRULE_H_*/
