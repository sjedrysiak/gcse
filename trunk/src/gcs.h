#ifndef GCS_H_
#define GCS_H_

#include "grammar.h"

class GCS
{
public:
    GCS();
    void run();
    virtual ~GCS();
private:
    Grammar mGrammar;
    QList<Sentence> mSentences;
};

#endif /*GCS_H_*/
