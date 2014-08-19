#include <GL/glew.h>
#include <GL/gl.h>

#include "Sampler.h"

Sampler::Sampler()
    : mSampler(0)
{
    glGenSamplers(1, &mSampler);
    glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

Sampler::~Sampler()
{
    glDeleteSamplers(1, &mSampler);
}
    
void Sampler::bind(int unit)
{
    glBindSampler(unit, mSampler);
    mUnit = unit;
}

