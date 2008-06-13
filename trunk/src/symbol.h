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

class TSymbol;

class NSymbol : public QString
{
public:
    NSymbol(const char* str = "");
    NSymbol(const QString& str);
    static NSymbol generateNew();
private:
    //to prevent implicit casting to TSymbol
    operator TSymbol() const;
    static QString lastUsed;
};

class TSymbol : public QString
{
public:
    TSymbol(const char* str = "");
    TSymbol(const QString& str);
    static TSymbol generateNew();
private:
    //to prevent implicit casting to NSymbol
    operator NSymbol() const;
    static QString lastUsed;
};

#endif /*SYMBOL_H_*/
