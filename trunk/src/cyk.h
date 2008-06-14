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

#ifndef CYK_H_
#define CYK_H_

#include <QString>
#include <QVector>
#include <QSet>
#include "nsymbol.h"
#include "nprodaction.h"

class Grammar;

class CYK
{
public:
	CYK();
	static bool parse(const Grammar& grammar, const QString& sentence, const QString& separator = "");
	~CYK();
private:
    static QList<NProdAction> getNActionsForCell(const QVector<QVector<QSet<NSymbol> > >& cykTable, int row, int col);
};

#endif /*CYK_H_*/