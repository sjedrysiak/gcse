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

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <QString>
#include <QHash>

///////////////////////////////////////////////////
//class NSymbol
///////////////////////////////////////////////////

class NSymbol: public QString
{
public:
	NSymbol(const QString& str = "") : QString(str), hash(qHash(str)) {}
	NSymbol(const char* str) : QString(str), hash(qHash(QString(str))) {}
	bool operator ==(const NSymbol& other) const
	{	return hash == other.hash;	}
	static NSymbol generateNew();
	static void resetGenerator()
	{	lastUsed = "";	}
protected:
	uint hash;
	static QString lastUsed;
};

///////////////////////////////////////////////////
//class TSymbol
///////////////////////////////////////////////////

class TSymbol: public QString
{
public:
	TSymbol(const QString& str) : QString(str), hash(qHash(str)) {}
	TSymbol(const char* str) : QString(str), hash(qHash(QString(str))) {}
	bool operator ==(const TSymbol& other) const
	{	return hash == other.hash;	}
protected:
	uint hash;
};

#endif /*SYMBOL_H_*/
