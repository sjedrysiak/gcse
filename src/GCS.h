#ifndef GCS_H_
#define GCS_H_

#include "Grammar.h"
#include <QList>
#include <QThread>
#include "Sentence.h"

class GCS : public QThread
{
public:
    GCS(const Grammar& grammar, const QList<Sentence> list);
    void run();
    virtual ~GCS();
protected:
	int threadNumber;
    Grammar mGrammar;
    QList<Sentence> mSentences;
    static int counter;
};

#endif /*GCS_H_*/
