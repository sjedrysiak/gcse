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

#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QTextCodec>
#include <cstdio>
#include <cstdlib>
//#include "mainwindow.h"
//#include "grammareditor.h"
#include "GCS.h"
#include "CYK.h"
#include "Random.h"

//void myMessageOutput(QtMsgType type, const char *msg)
//{
//	switch (type)
//	{
//		case QtDebugMsg:
//			fprintf(stderr, "Debug: %s\n", msg);
//			break;
//		case QtWarningMsg:
//			fprintf(stderr, "Warning: %s\n", msg);
//			break;
//		case QtCriticalMsg:
//			fprintf(stderr, "Critical: %s\n", msg);
//			break;
//		case QtFatalMsg:
//			fprintf(stderr, "Fatal: %s\n", msg);
//			abort();
//	}
//}

int main()//int argc, char *argv[])
{
	Random::init();
//	qInstallMsgHandler(myMessageOutput);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

//	QApplication app(argc, argv);
//	QLabel label("taka sobie labelka jestem");
//	label.show();
	GCS gcs;
	gcs.run();
//	return app.exec();
	return EXIT_SUCCESS;
}
