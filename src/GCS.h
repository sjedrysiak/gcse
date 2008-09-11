#ifndef GCS_H_
#define GCS_H_

#include "Grammar.h"
#include <QList>
#include <QThread>
#include <QMutex>
#include "Sentence.h"

class MainWindow;

class GCS : public QThread
{
	Q_OBJECT
public:
    GCS(const Grammar& grammar, const QList<Sentence> list, MainWindow& parent);
    void run();
    Grammar getBestGrammar()
    {
    	return bestGrammar;
    }
    void sendRules(QList<NClassifier> list);
    virtual ~GCS();
	int threadNumber;
    static int counter;
signals:
	void stepChanged(int value);
	void iterChanged(int value);
	void parsedSentenceChanged(int value);
protected:
    void exchangeRules();
	void takeRules();
    Grammar initGrammar;
    Grammar tempGrammar;
    Grammar bestGrammar;
    QList<Sentence> mSentences;
    MainWindow& parent;
    QList<NClassifier> exchangeBuffer;
    bool bufferBusy;
    QMutex mutex;
public slots:
	void nextSentence(int value);
};

#endif /*GCS_H_*/
