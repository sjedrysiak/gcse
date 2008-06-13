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

#include "prodaction.h"

///////////////////////////////////////////////////
//class ProdAction
///////////////////////////////////////////////////

//ProdAction::~ProdAction()
//{
//}

///////////////////////////////////////////////////
//class NProdAction
///////////////////////////////////////////////////

NProdAction::NProdAction()
{
}

NProdAction::NProdAction(const NSymbol& first, const NSymbol& second)
{
    this->mFirstSymbol = first;
    this->mSecondSymbol = second;
}

const NSymbol& NProdAction::firstSymbol() const
{
    return this->mFirstSymbol;
}

void NProdAction::setFirstSymbol(const NSymbol& s)
{
    this->mFirstSymbol = s;
}

const NSymbol& NProdAction::secondSymbol() const
{
    return this->mSecondSymbol;
}

void NProdAction::setSecondSymbol(const NSymbol& s)
{
    this->mSecondSymbol = s;
}

bool NProdAction::operator ==(const NProdAction& other) const
{
    return this->mFirstSymbol == other.mFirstSymbol && this->mSecondSymbol == other.mSecondSymbol;
}

//ProdAction* NProdAction::clone() const
//{
//    return new NProdAction(*this);
//}

//bool NProdAction::isTerminal() const
//{
//    return false;
//}

NProdAction::operator QString() const
{
    return this->mFirstSymbol + " | " + this->mSecondSymbol;
}

NProdAction::~NProdAction()
{
}

///////////////////////////////////////////////////
//class TProdAction
///////////////////////////////////////////////////

TProdAction::TProdAction()
{
}

TProdAction::TProdAction(const TSymbol& s)
{
    this->mSymbol = s;
}

const TSymbol& TProdAction::symbol() const
{
    return this->mSymbol;
}

void TProdAction::setSymbol(const TSymbol& s)
{
    this->mSymbol = s;
}

bool TProdAction::operator ==(const TProdAction& other) const
{
    return this->mSymbol == other.mSymbol;
}

//ProdAction* TProdAction::clone() const
//{
//    return new TProdAction(*this);
//}

//bool TProdAction::isTerminal() const
//{
//    return true;
//}

TProdAction::operator QString() const
{
    return this->mSymbol;
}

TProdAction::~TProdAction()
{
}
