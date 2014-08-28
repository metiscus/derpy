#include "Tokenizer.h"

#include <sstream>

Tokenizer::Tokenizer(const std::string& delimiters)
    : mDelimiters(delimiters)
    , mInputItr(0)
{
    ;
}

Tokenizer& Tokenizer::operator<<(std::string& lhs)
{
    mInput += lhs;
    return *this;
}

Tokenizer& Tokenizer::operator>>(std::string& lhs)
{
    const size_t inputLength = mInput.length();
    for( ; mInputItr<inputLength; ++mInputItr )
    {
        for(size_t ii=0; ii<mDelimiters.length(); ++ii)
        {
            if(mInput[mInputItr]==mDelimiters[ii])
            {
                ++mInputItr;
                return *this;
            }
        }
        
        lhs += mInput[mInputItr];        
    }
    return *this;
}

void Tokenizer::setDelimiters(const std::string& delimiters)
{
    mDelimiters = delimiters;
}

void Tokenizer::clear()
{
    mInput = "";
    mInputItr = 0;
}

void Tokenizer::rewind()
{
    mInputItr = 0;
}

size_t Tokenizer::remaining() const
{
    return mInput.size() - mInputItr;
}
