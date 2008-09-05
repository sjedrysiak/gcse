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
public:
    GCS(const Grammar& grammar, const QList<Sentence> list, MainWindow& parent);
    void run();
    Grammar getGrammar()
    {
    	return outGrammar;
    }
    void sendRules(QList<NClassifier> list);
    virtual ~GCS();
protected:
    void exchangeRules();
	void takeRules();
	int threadNumber;
    Grammar initGrammar;
    Grammar outGrammar;
    QList<Sentence> mSentences;
    MainWindow& parent;
    QList<NClassifier> exchangeBuffer;
    bool bufferBusy;
    static int counter;
    QMutex mutex;
};

#endif /*GCS_H_*/
