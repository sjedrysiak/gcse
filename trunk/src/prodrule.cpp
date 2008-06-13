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

#include "prodrule.h"

///////////////////////////////////////////////////
//class NProdRule
///////////////////////////////////////////////////

NProdRule::NProdRule()
{
}

NProdRule::NProdRule(const ProdCondition& cond, const NProdAction& act)
{
    this->mCondition = cond;
    this->mAction = act;
}

const ProdCondition& NProdRule::condition() const
{
    return this->mCondition;
}

void NProdRule::setCondition(const ProdCondition& cond)
{
    this->mCondition = cond;
}

const NProdAction& NProdRule::action() const
{
    return this->mAction;
}

void NProdRule::setAction(const NProdAction& act)
{
    this->mAction = act;
}

bool NProdRule::operator ==(const NProdRule& other) const
{
    return this->mCondition == other.mCondition && this->mAction == other.mAction;
}

NProdRule::operator QString() const
{
    return this->mCondition + " -> " + this->mAction;
}

NProdRule::~NProdRule()
{
}

///////////////////////////////////////////////////
//class TProdRule
///////////////////////////////////////////////////

TProdRule::TProdRule()
{
}

TProdRule::TProdRule(const ProdCondition& cond, const TProdAction& act)
{
    this->mCondition = cond;
    this->mAction = act;
}

const ProdCondition& TProdRule::condition() const
{
    return this->mCondition;
}

void TProdRule::setCondition(const ProdCondition& cond)
{
    this->mCondition = cond;
}

const TProdAction& TProdRule::action() const
{
    return this->mAction;
}

void TProdRule::setAction(const TProdAction& act)
{
    this->mAction = act;
}

bool TProdRule::operator ==(const TProdRule& other) const
{
    return this->mCondition == other.mCondition && this->mAction == other.mAction;
}

TProdRule::operator QString() const
{
    return this->mCondition + " -> " + this->mAction;
}

TProdRule::~TProdRule()
{
}
