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

#include "Condition.h"

NCondition::NCondition(const NSymbol& first, const NSymbol& second) :
	mFirstSymbol(first), mSecondSymbol(second)
{
}

const NSymbol& NCondition::firstSymbol() const
{
	return this->mFirstSymbol;
}

void NCondition::setFirstSymbol(const NSymbol& s)
{
	this->mFirstSymbol = s;
}

const NSymbol& NCondition::secondSymbol() const
{
	return this->mSecondSymbol;
}

void NCondition::setSecondSymbol(const NSymbol& s)
{
	this->mSecondSymbol = s;
}

bool NCondition::operator ==(const NCondition& other) const
{
	return this->mFirstSymbol == other.mFirstSymbol && this->mSecondSymbol == other.mSecondSymbol;
}

NCondition::operator QString() const
{
	return this->mFirstSymbol + "|" + this->mSecondSymbol;
}

NCondition::~NCondition()
{
}

TCondition::TCondition(const TSymbol& s) :
	mSymbol(s)
{
}

const TSymbol& TCondition::symbol() const
{
	return this->mSymbol;
}

void TCondition::setSymbol(const TSymbol& s)
{
	this->mSymbol = s;
}

bool TCondition::operator ==(const TCondition& other) const
{
	return this->mSymbol == other.mSymbol;
}

TCondition::operator QString() const
{
	return this->mSymbol;
}

TCondition::~TCondition()
{
}
