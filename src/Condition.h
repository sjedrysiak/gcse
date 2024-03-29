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

#ifndef CONDITION_H_
#define CONDITION_H_

#include "Symbol.h"

/**
 * classifier condition
 */
class NCondition
{
public:
	NCondition(const NSymbol& first, const NSymbol& second) :
		firstSymbol(first), secondSymbol(second) {}
	bool operator ==(const NCondition& other) const
	{	return firstSymbol == other.firstSymbol && secondSymbol == other.secondSymbol;	}
	QString toString() const
	{	return firstSymbol + "|" + secondSymbol;	}

	NSymbol firstSymbol;
	NSymbol secondSymbol;
};

class TCondition
{
public:
	TCondition(const TSymbol& s) : symbol(s) {}
	bool operator ==(const TCondition& other) const
	{	return symbol == other.symbol;	}
	QString toString() const
	{	return symbol;	}

	TSymbol symbol;
};

#endif /*CONDITION_H_*/
