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

#include "grammar.h"
#include "random.h"
#include <QStringList>
#include <QList>

Grammar::Grammar(const NSymbol& start, const NSymbol& universal) :
    S(start), Su(universal)
{
    this->N << this->S;
    this->N << this->Su;
    this->generateGrammar();
}

void Grammar::generateGrammar()
{
    NSymbol A("A"), B("B"), C("C"), D("D");
    TSymbol a("a"), b("b"), c("c");
    this->N << A;
    this->N << B;
    this->N << C;
    this->N << D;
    this->T << a;
    this->T << b;
    this->T << c;

    this->PN << NClassifier( NProdRule( ProdCondition(this->S), NProdAction(A,B) ) ); // S -> AB
    this->PN << NClassifier( NProdRule( ProdCondition(this->S), NProdAction(A,C) ) ); // S -> AC
    this->PN << NClassifier( NProdRule( ProdCondition(C), NProdAction(this->S,B) ) ); // C -> SB
    this->PN << NClassifier( NProdRule( ProdCondition(B), NProdAction(B,B) ) ); // B -> BB
    this->PT << TClassifier( TProdRule( ProdCondition(C), TProdAction(a) ) ); // C -> a
    this->PT << TClassifier( TProdRule( ProdCondition(B), TProdAction(b) ) ); // B -> b
    this->PT << TClassifier( TProdRule( ProdCondition(A), TProdAction(a) ) ); // A -> a
}

bool Grammar::parse(const QString& sentence, const QString& separator)
{
    QStringList words(sentence.split(separator, QString::SkipEmptyParts));
    QVector<TSymbol> terminals;
    foreach (QString str, words)
    {
        terminals << TSymbol(str);
    }
    int size = terminals.size();
    CYKTable cykTable(size);
    for (int row = 0; row < size; row++) //set rows' sizes
    {
        cykTable[row].resize(size - row);
    }
    QList<NSymbol> M;//list of matched classifiers' conditions
    for (int col = 0; col < size; col++) //set first row
    {
        M = this->getMatchingClassifiers( TProdAction(terminals[col]) );
        if (M.size() == 0)//there is no terminal prod for current word
        {
            M << this->coveringTerminal(terminals[col]);
            if (false)//TODO zmienna decyduj�ca o uruchomieniu operatora pokrycia uniwersalnego
            {
                M << this->Su;
            }
            if (/*mozna uzyc startowy*/false && size == 1 /*&& zdanie pozytywne*/)//TODO zmienna do operatora pokrycia startowego
            {
                //TODO klasa sentence z polem pozytywne/negatywne
                this->coveringStart(terminals[col]);
                M << this->S;
            }
        }
        cykTable[0][col] = M.toSet();
    }
    M.clear();
    for (int row = 1; row < size; row++)
    {
        for (int col = 0; col < size - row; col++)
        {
            QList<NProdAction> D = getConditionsForCykCell(cykTable, row, col);
            foreach (NProdAction condition, D)
            {
                M = this->getMatchingClassifiers(condition);
                if (M.size() == 0 /*&& zdanie pozytywne*/)//TODO klasa sentence
                {
                    if (Random::rand() < 0.01)//TODO zmienna dla pokrycia agresywnego
                    {
                        M << this->coveringAggressive(condition);
                    }
                    if (/*zmienna dla pokrycia pelnego &&*/ row == size-1 && col == 0)//TODO zmienna dla pokrycia pelnego
                    {
                        this->coveringFull(condition);
                        M << this->S;
                    }
                }
                cykTable[row][col] += M.toSet();
            }
        }
    }
    return cykTable[size-1][0].contains(this->S);
}

QList<NSymbol> Grammar::getMatchingClassifiers(const NProdAction& condition) const
{
    QList<NSymbol> list;
    foreach (NClassifier cl, this->PN)
    {
        if (cl.condition() == condition)
        {
            list.append( cl.action().symbol() );
        }
    }
    return list;
}

QList<NSymbol> Grammar::getMatchingClassifiers(const TProdAction& condition) const
{
    QList<NSymbol> list;
    foreach (TClassifier cl, this->PT)
    {
        if (cl.condition() == condition)
        {
            list.append( cl.action().symbol() );
        }
    }
    return list;
}

QList<NProdAction> Grammar::getConditionsForCykCell(const CYKTable& cykTable, int row, int col) const
{
    QList<NProdAction> list;
    for (int r = 0; r < row; r++)
    {
        foreach (NSymbol s1, cykTable[r][col])
        {
            foreach (NSymbol s2, cykTable[row-1-r][col+1+r])
            {
                list.append( NProdAction(s1, s2) );
            }
        }
    }
    return list;
}

//covering operators
NSymbol Grammar::coveringTerminal(const TSymbol& term)
{
    NSymbol newSymbol = NSymbol::generateNew();
    this->addClNormal( TClassifier( TProdRule( ProdCondition(newSymbol), TProdAction(term) ) ) );
    //TODO dorobi� zmienn� decyduj�c� o wywo�aniu operatora pokrycia uniwersalnego
    if(false)
    {
        this->coveringUniversal(term);
    }
    return newSymbol;
}

void Grammar::coveringUniversal(const TSymbol& term)
{
    this->addClNormal( TClassifier( TProdRule(ProdCondition(this->Su), TProdAction(term)) ) );
}

void Grammar::coveringStart(const TSymbol& term)
{
    this->addClWithCrowding( TClassifier( TProdRule(ProdCondition(this->S), TProdAction(term)) ), this->PT );
}

void Grammar::coveringFull(const NProdAction& cond)
{
    this->addClWithCrowding( NClassifier( NProdRule( ProdCondition(this->S), NProdAction(cond) ) ), this->PN );
}

NSymbol Grammar::coveringAggressive(const NProdAction& cond)
{
    NSymbol newSymbol = NSymbol::generateNew();
    this->addClWithCrowding( NClassifier( NProdRule( ProdCondition(newSymbol), NProdAction(cond) ) ), this->PN );
    return newSymbol;
}

//adding methods
void Grammar::addSymbol(const NSymbol &s)
{
    this->N << s;
}

void Grammar::addSymbol(const TSymbol &s)
{
    this->T << s;
}

void Grammar::addClNormal(const NClassifier& cl)
{
    this->addSymbol(cl.prodCondition().symbol());
    this->addSymbol(cl.prodAction().firstSymbol());
    this->addSymbol(cl.prodAction().secondSymbol());
    this->PN << cl;
}

void Grammar::addClNormal(const TClassifier& cl)
{
    this->addSymbol(cl.prodCondition().symbol());
    this->addSymbol(cl.prodAction().symbol());
    this->PT << cl;
}

void Grammar::addClWithCrowding(const NClassifier& newCl, QSet<NClassifier>& set)
{
    QVector<NClassifier> vect(set.toList().toVector());

    QSet<NClassifier> K;
    for (int i = 0; i < 5; i++)//TODO zmienna rozmiar populacji z najgorszymi do �cisku
    {
        NClassifier worst = vect[Random::rand(vect.size())];
        QSet<NClassifier> W;
        W << worst;
        while (W.size() < 5)//TODO zmienna rozmiar populacji scisku
        {
            NClassifier temp = vect[Random::rand(vect.size())];
            W << temp;
            if (temp.fitness() < worst.fitness())
            {
                worst = temp;
            }
        }
        K << worst;
    }

    NClassifier mostSimilar = K.toList()[0];
    foreach (NClassifier temp, K)
    {
        if (temp.similar(newCl) > mostSimilar.similar(newCl))
        {
            mostSimilar = temp;
        }
    }
    set.remove(mostSimilar);
    set << newCl;
}

void Grammar::addClWithCrowding(const TClassifier& newCl, QSet<TClassifier>& set)
{
    QVector<TClassifier> vect(set.toList().toVector());

    QSet<TClassifier> K;
    for (int i = 0; i < 5; i++)//TODO zmienna rozmiar populacji z najgorszymi do �cisku
    {
        TClassifier worst = vect[Random::rand(vect.size())];
        QSet<TClassifier> W;
        W << worst;
        while (W.size() < 5)//TODO zmienna rozmiar populacji scisku
        {
            TClassifier temp = vect[Random::rand(vect.size())];
            W << temp;
            if (temp.fitness() < worst.fitness())
            {
                worst = temp;
            }
        }
        K << worst;
    }

    TClassifier mostSimilar = K.toList()[0];
    foreach (TClassifier temp, K)
    {
        if (temp.similar(newCl) > mostSimilar.similar(newCl))
        {
            mostSimilar = temp;
        }
    }
    set.remove(mostSimilar);
    set << newCl;
}

//GA part
//selection
NClassifier Grammar::selectionRoulette() const
{
    double sum = 0;
    foreach (NClassifier cl, this->PN)
    {
        sum += cl.fitness();
    }
    double random = Random::rand() * sum;
    sum = 0.0;
    NClassifier winner;
    foreach (NClassifier cl, this->PN)
    {
        sum += cl.fitness();
        if (random <= sum)
        {
            winner = cl;
            break;
        }
    }
    return winner;
}

NClassifier Grammar::selectionTournament() const
{
    //TODO doda� zmienn� rozmiar populacji turniejowej
    int turnamentSize = 5;
    NClassifier bestCl;

    QList<NClassifier> PNList( this->PN.toList() );
    int PNSize = PNList.size();

    if (turnamentSize >= PNSize)
    {
        bestCl = PNList[0];
        foreach (NClassifier cl, PNList)
        {
            if (cl.fitness() > bestCl.fitness())
            {
                bestCl = cl;
            }
        }
    }
    else
    {
        QSet<NClassifier> turnamentSet;
        bestCl = PNList[Random::rand(PNSize)];
        turnamentSet << bestCl;

        while(turnamentSet.size() < turnamentSize)
        {
            NClassifier cl = PNList[Random::rand(PNSize)];
            turnamentSet << cl;
            if (cl.fitness() > bestCl.fitness())
            {
                bestCl = cl;
            }
        }
    }
    return bestCl;
}

NClassifier Grammar::selectionRandom() const
{
    return this->PN.toList()[Random::rand(this->N.size())];
}

//GA operators
void Grammar::gaCrossover(NClassifier& first, NClassifier& second) const
{
    NClassifier copyFirst(first);
    if ( qrand() % 2)
    {
        first.setProdAction( NProdAction(second.prodAction().firstSymbol(), first.prodAction().secondSymbol()) );
        second.setProdAction( NProdAction(copyFirst.prodAction().firstSymbol(), second.prodAction().secondSymbol()) );
    }
    else
    {
        first.setProdAction( NProdAction(first.prodAction().firstSymbol(), second.prodAction().secondSymbol()) );
        second.setProdAction( NProdAction(second.prodAction().firstSymbol(), copyFirst.prodAction().secondSymbol()) );
    }
    first.setProdCondition( ProdCondition(second.prodCondition().symbol()) );
    second.setProdCondition( ProdCondition(copyFirst.prodCondition().symbol()) );
}

void Grammar::gaInversion(NClassifier& cl) const
{
    cl.setProdAction( NProdAction(cl.prodAction().secondSymbol(), cl.prodAction().firstSymbol()) );
}

void Grammar::gaMutation(NClassifier& cl) const
{
    QList<NSymbol> symbols(this->N.toList());
    //TODO doda� parametr decyduj�cy o mutacji
    if( Random::rand() < 0.01 )
    {//mutation of left side
        int i = Random::rand( symbols.size() );
        cl.setProdCondition( ProdCondition(symbols[i]) );
    }
    if( Random::rand() < 0.01 )
    {//mutation of first right symbol
        int i = Random::rand( symbols.size() );
        cl.setProdAction( NProdAction(symbols[i], cl.prodAction().secondSymbol()) );
    }
    if( Random::rand() < 0.01 )
    {//mutation of second right symbol
        int i = Random::rand( symbols.size() );
        cl.setProdAction( NProdAction(cl.prodAction().firstSymbol(), symbols[i]) );
    }
}

void Grammar::ga()
{
    NClassifier cl1, cl2;
    switch(1 /*rodzaj selekcji dla cl1*/)//TODO zmienna rodzaj selekcji dla cl1, dorobic enuma rodzaj selekcji
    {
    case 1:
        cl1 = this->selectionRandom();
        break;
    case 2:
        cl1 = this->selectionRoulette();
        break;
    case 3:
        cl1 = this->selectionTournament();
        break;
    default:
        cl1 = this->selectionRoulette();
        break;
    }
    switch(2 /*rodzaj selekcji dla cl2*/)//TODO zmienna rodzaj selekcji dla cl2, dorobic enuma rodzaj selekcji
    {
    case 1:
        cl2 = this->selectionRandom();
        break;
    case 2:
        cl2 = this->selectionRoulette();
        break;
    case 3:
        cl2 = this->selectionTournament();
        break;
    default:
        cl2 = this->selectionRoulette();
        break;
    }

    //Crossover
    if (Random::rand() < 0.01)//TODO zmienna decyduj�ca o krzy�owaniu
    {
        this->gaCrossover(cl1, cl2);
    }

    //Mutation
    this->gaMutation(cl1);
    this->gaMutation(cl2);

    //Inversion
    if (Random::rand() < 0.01)//TODO zmienna decyduj�ca o inwersji cl1
    {
        this->gaInversion(cl1);
    }
    if (Random::rand() < 0.01)//TODO zmienna decyduj�ca o inwersji cl2
    {
        this->gaInversion(cl2);
    }

    //Elite population
    QList<NClassifier> elite = this->PN.toList();
    qSort(elite);
    while (elite.size() > 5/*rozmiar populacji elitarnej*/)//TODO zmienna rozmiar populacji elitarnej
    {
        elite.removeFirst();
    }

    QSet<NClassifier> temp = this->PN - elite.toSet();
    this->addClWithCrowding(cl1, temp);
    this->addClWithCrowding(cl2, temp);
    this->PN = temp + elite.toSet();
}

Grammar::~Grammar()
{
}
