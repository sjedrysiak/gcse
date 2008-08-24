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

#include "Symbol.h"

///////////////////////////////////////////////////
//class NSymbol
///////////////////////////////////////////////////

QString NSymbol::lastUsed = "";

NSymbol::NSymbol(const QString& str) :
	QString(str)
{}

NSymbol::NSymbol(const char* str) :
	QString(str)
{}

NSymbol NSymbol::generateNew()
{
	if (lastUsed.size() == 0)
	{
		lastUsed = "A";
	}
	else
	{
		int i;
		for (i = lastUsed.size() - 1; i >= 0; i--)
		{
			if (lastUsed[i] == 'Z')
			{
				lastUsed[i] = 'A';
			}
			else //current char != 'Z', just increment char
			{
				lastUsed[i] = lastUsed[i].toAscii() + 1;
				break;
			}
		}
		if (i == -1) //all chars == 'Z'
		{
			lastUsed.append('A');
		}
	}
	return lastUsed;
}

void NSymbol::resetGenerator()
{
	lastUsed = "";
}
///////////////////////////////////////////////////
//class TSymbol
///////////////////////////////////////////////////

TSymbol::TSymbol(const QString& str) :
	QString(str)
{
}

TSymbol::TSymbol(const char* str) :
	QString(str)
{
}
