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

#ifndef GA_H_
#define GA_H_

class GA
{
public:
    enum SelectionType
    {
        ROULETTE,
        TOURNAMENT,
        RANDOM
    };

    static void evolve(Grammar& g);
private:
    //selection operators
    static NClassifier selectionRoulette(const Grammar& g);
    static NClassifier selectionTournament(const Grammar& g);
    static NClassifier selectionRandom(const Grammar& g);

    //genetic operators
    static void crossover(NClassifier& first, NClassifier& second);
    static void inversion(NClassifier& cl);
    static void mutation(NClassifier& cl);

    //parameters
    static SelectionType selectionCl1;
    static SelectionType selectionCl2;
    static float pCrossover;
    static float pMutation;
    static float pInversion;
    static int eliteSize;
    static int tournamentSize;
};

#endif /*GA_H_*/
