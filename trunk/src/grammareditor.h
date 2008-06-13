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

#ifndef GRAMMAREDITOR_H
#define GRAMMAREDITOR_H

#include <QtGui/QWidget>
#include "ui_grammareditor.h"
#include "grammar.h"

class GrammarEditor : public QWidget, private Ui::GrammarEditor
{
    Q_OBJECT

public:
    GrammarEditor(Grammar* grammar, QWidget *parent = 0);
    void setupActions();
    ~GrammarEditor();
protected:
    GrammarEditor(const GrammarEditor& other);
protected slots:
    void addProdRule();
private:
    Grammar* g;
};

#endif // GRAMMAREDITOR_H
