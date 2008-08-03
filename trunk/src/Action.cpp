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

#include "Action.h"

Action::Action(const NSymbol& s) :
	mSymbol(s)
{
}

const NSymbol& Action::symbol() const
{
    return this->mSymbol;
}

void Action::setSymbol(const NSymbol& s)
{
    this->mSymbol = s;
}

bool Action::operator ==(const Action& other) const
{
    return this->mSymbol == other.mSymbol;
}

Action::operator QString() const
{
    return this->mSymbol;
}

Action::~Action()
{}