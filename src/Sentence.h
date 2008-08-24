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

#ifndef SENTENCE_H_
#define SENTENCE_H_

#include <QString>
#include <QList>
#include "Symbol.h"

class Sentence : public QList<TSymbol>
{
public:
	Sentence(const QString& str = "", bool isPositive = true);
	Sentence(const char* str, bool isPositive = true);
	bool isPositive() const;
	void setPositive(bool isPositive);
	operator QString() const;
	virtual ~Sentence(){}

    static QString wordSeparator;
protected:
	void split(const QString& str);

    bool mIsPositive;
};

#endif /*SENTENCE_H_*/
