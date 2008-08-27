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

#ifndef RANDOM_H_
#define RANDOM_H_

#include <cstdlib>
#include <time.h>
#include <QMutex>
#include <QMutexLocker>

class Random
{
public:
	/**
	 * returns random integer from 0 to n-1
	 */
	static int rand(unsigned int n)
	{
		QMutexLocker locker(&mutex);
		return ::rand() % n;
	}
	/**
	 * returns random double from [0,1)
	 */
	static double rand()
	{
		QMutexLocker locker(&mutex);
		return ::rand() / (RAND_MAX + 1.0);
	}
	/**
	 * initialize random generator by calling srand()
	 */
	static void srand()
	{
		QMutexLocker locker(&mutex);
		::srand(time(NULL));
	}
private:
	static QMutex mutex;
};

#endif /*RANDOM_H_*/
