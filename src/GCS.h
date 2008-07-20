#ifndef GCS_H_
#define GCS_H_

#include "Grammar.h"
#include <QList>
#include "Sentence.h"

class GCS
{
public:
    GCS();
    void run();
    virtual ~GCS();
protected:
    Grammar mGrammar;
    QList<Sentence> mSentences;
};

#endif /*GCS_H_*/
