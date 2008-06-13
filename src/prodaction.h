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

#ifndef PRODACTION_H_
#define PRODACTION_H_

#include "symbol.h"

//class ProdAction
//{
//public:
//    virtual ProdAction* clone() const = 0;
//    virtual bool isTerminal() const = 0;
//    virtual operator QString() const = 0;
//    virtual ~ProdAction();
//};

class NProdAction// : public ProdAction
{
public:
    NProdAction();
    NProdAction(const NSymbol& first, const NSymbol& second);
    const NSymbol& firstSymbol() const;
    void setFirstSymbol(const NSymbol& s);
    const NSymbol& secondSymbol() const;
    void setSecondSymbol(const NSymbol& s);
    bool operator ==(const NProdAction& other) const;
    //ProdAction* clone() const;
    //bool isTerminal() const;
    operator QString() const;
    ~NProdAction();
private:
    NSymbol mFirstSymbol;
    NSymbol mSecondSymbol;
};

class TProdAction// : public ProdAction
{
public:
    TProdAction();
    TProdAction(const TSymbol& s);
    const TSymbol& symbol() const;
    void setSymbol(const TSymbol& s);
    bool operator ==(const TProdAction& other) const;
    //ProdAction* clone() const;
    //bool isTerminal() const;
    operator QString() const;
    ~TProdAction();
private:
    TSymbol mSymbol;
};

#endif /*PRODACTION_H_*/
