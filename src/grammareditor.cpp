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

#include "grammareditor.h"

GrammarEditor::GrammarEditor(Grammar* grammar, QWidget *parent) :
    QWidget(parent)
{
    this->setupUi(this);
    this->setupActions();
    this->g = grammar;
}

void GrammarEditor::setupActions()
{
    connect(this->btnAdd, SIGNAL(clicked()), this, SLOT(addProdRule()));
}

void GrammarEditor::addProdRule()
{
    //ProdCondition cond( NSymbol(this->edtCondition->text()) );
    //NSymbol first = this->edtFirstSymbol->text();
    //NSymbol second = this->edtSecondSymbol->text();
    //this->g->addNSymbol(cond.getSymbol());
    //this->g->addNSymbol(first);
    //this->g->addNSymbol(second);
    //this->g->addNProdRule(NProdRule(cond,NProdAction(first, second)));
    //this->list->clear();
    //foreach (NProdRule rule, g->NProdRules())
    //{
    //    this->list->addItem(rule);
    //}
}

GrammarEditor::~GrammarEditor()
{
}
