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
    void readSentences(const QString& path);
protected:
    void setParams();
    Grammar mGrammar;
    QList<Sentence> mSentences;
};

#endif /*GCS_H_*/
