/*
 * tests.cpp
 *
 *  Created on: 2008-07-19
 *      Author: sylver
 */

#include <QtTest/QtTest>

#include "Symbol.h"
#include "Sentence.h"
#include "Condition.h"
#include "Action.h"
#include "Classifier.h"

class Tests: public QObject
{
	Q_OBJECT
private slots:
	void nsymbol();
	void tsymbol();
	void sentence();
	void ncondition();
	void tcondition();
	void action();
	void nclassifier();
	void tclassifier();
};

void Tests::nsymbol()
{
	NSymbol a;
	QVERIFY(a == "A");
	NSymbol b;
	QVERIFY(b == "B");
	a = b;
	QVERIFY(a == b);
}

void Tests::tsymbol()
{
	TSymbol a("a");
	QVERIFY(a == "a");
	TSymbol b("b");
	QVERIFY(b == "b");
	a = b;
	QVERIFY(a == b);
	a = "x";
	QVERIFY(a == "x");
}

void Tests::sentence()
{
	Sentence s;
	QVERIFY((QString)s == "" && s.isPositive());
	s = "test 1";
	s.setPositive(false);
	QVERIFY((QString)s == "test 1" && s.isPositive() == false);
	QString str("test 3");
	Sentence s3(str);
	QVERIFY((QString)s3 == "test 3" && s3.isPositive());
	Sentence s4("test 4", false);
	QVERIFY((QString)s4 == "test 4" && s4.isPositive() == false);
	QVERIFY(Sentence::wordSeparator() == " ");
	Sentence::setWordSeparator(",");
	QVERIFY(Sentence::wordSeparator() == ",");
	Sentence s5("test,5,6");
	QVERIFY((QString)s5 == "test,5,6" && s5[0] == "test" && s5[1] == "5" && s5[2] == "6");
	s = "test,1";
	s3 = "test,1";
	QVERIFY(s == s3);
}

void Tests::ncondition()
{
	NSymbol c,d;
	NCondition cond(c,d), cond2(d,c);
	QVERIFY(QString(cond) == "C|D" && cond.firstSymbol() == c && cond.secondSymbol() == d);
	cond.setFirstSymbol(d);
	QVERIFY(QString(cond) == "D|D" && cond.firstSymbol() == d && cond.secondSymbol() == d);
	cond.setSecondSymbol(c);
	QVERIFY(QString(cond) == "D|C" && cond.firstSymbol() == d && cond.secondSymbol() == c);
	QVERIFY(cond == cond2);
}

void Tests::tcondition()
{
	TSymbol a("a"),x("x");
	TCondition cond(a),cond2(x);
	QVERIFY(QString(cond) == "a" && cond.symbol() == a);
	cond.setSymbol(x);
	QVERIFY(QString(cond) == "x" && cond.symbol() == x);
	QVERIFY(cond == cond2);
}

void Tests::action()
{
	NSymbol e,f;
	Action a(e),b(f);
	QVERIFY(QString(a) == "E" && a.symbol() == e);
	a.setSymbol(f);
	QVERIFY(QString(a) == "F" && a.symbol() == f);
	QVERIFY(a == b);
}

void Tests::nclassifier()
{
	NSymbol g, h, i, j, k, l;
	NCondition cond(g,h), cond2(j,k);
	Action act(i), act2(l);
	NClassifier cl(cond,act), cl2(cond2, act2);
	QVERIFY(QString(cl) == "G|H<-I/0" && cl.condition() == cond && cl.action() == act);
	cl.setCondition(cond2);
	cl.setAction(act2);
	QVERIFY(QString(cl) == "J|K<-L/0" && cl.condition() == cond2 && cl.action() == act2);
	QVERIFY(cl == cl2);
}

void Tests::tclassifier()
{
	NSymbol m, n;
	TSymbol a("a"), b("b");
	TCondition cond(a), cond2(b);
	Action act(m), act2(n);
	TClassifier cl(cond,act), cl2(cond2, act2);
	QVERIFY(QString(cl) == "a<-M/0" && cl.condition() == cond && cl.action() == act);
	cl.setCondition(cond2);
	cl.setAction(act2);
	QVERIFY(QString(cl) == "b<-N/0" && cl.condition() == cond2 && cl.action() == act2);
	QVERIFY(cl == cl2);
}

QTEST_MAIN(Tests)
#include "tests.moc"
