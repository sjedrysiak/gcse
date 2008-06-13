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

#include "mainwindow.h"

#include <QMessageBox>
//#include "grammar.h"
//#include "cyk.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setupUi(this);
    this->setupActions();
}

void MainWindow::setupActions()
{
    connect(action_close, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));
}

void MainWindow::about()
{
    QString system = tr("Unknown");
#ifdef Q_WS_X11
    system = tr("Linux");
#endif

#ifdef Q_WS_WIN
    system = tr("Windows");
#endif
    QMessageBox::about(this, tr("About SGCS"), tr("SGCS 1.0\nA Qt application example.\n"
            "Program working on platform %1.\n"
            "© 2008 Sylwester Jędrysiak").arg(system));
}

MainWindow::~MainWindow()
{
}
