#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "Noncopyable.h"

class Sampler : public noncopyable
{
public:
    Sampler();
    ~Sampler();
    
    void bind(int unit);
    
private:
    unsigned int mUnit;
    unsigned int mSampler;
    
    friend class Uniform;
};

#endif /* SAMPLER_H_ */
