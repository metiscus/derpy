#ifndef SAMPLER_H_
#define SAMPLER_H_

class Sampler
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
