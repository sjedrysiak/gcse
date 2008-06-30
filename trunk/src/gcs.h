#ifndef GCS_H_
#define GCS_H_

class GCS
{
public:
    GCS();
    void run();
    virtual ~GCS();
private:
    Grammar mGrammar;
};

#endif /*GCS_H_*/
