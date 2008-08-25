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

#include "Sentence.h"
#include <QStringList>

QString Sentence::wordSeparator = " ";

Sentence::Sentence(const QString& str, bool isPositive) :
	QList<TSymbol> (), mIsPositive(isPositive)
{
	if (!str.isEmpty())
	{
		split(str);
	}
}

Sentence::Sentence(const char* str, bool isPositive) :
	QList<TSymbol> (), mIsPositive(isPositive)
{
	split(str);
}

bool Sentence::isPositive() const
{
	return mIsPositive;
}

void Sentence::setPositive(bool isPositive)
{
	mIsPositive = isPositive;
}

QString Sentence::toString() const
{
	QString ret;
	for (int i = 0, size = this->size(); i < size; i++)
	{
		ret += (*this)[i];
		if (i < size-1)
		{
			ret += Sentence::wordSeparator;
		}
	}
	if (!ret.isEmpty())
	{
		ret += QString(mIsPositive ? "(positive)" : "(negative)");
	}
	return ret;
}

void Sentence::split(const QString& str)
{
	QStringList list = str.split(Sentence::wordSeparator, QString::SkipEmptyParts);
	for (int i = 0, size = list.size(); i < size; i++)
	{
		append(TSymbol(list[i]));
	}
}
