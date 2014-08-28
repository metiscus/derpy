#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <sstream>

class Tokenizer
{
public:
    Tokenizer(const std::string& delimiters=",");
    Tokenizer& operator<<(std::string& lhs);
    Tokenizer& operator>>(std::string& rhs);
    
    template<typename T>
    Tokenizer& operator>>(T& rhs)
    {
        std::string token;
        *this>>token;
        
        std::stringstream ss;
        ss<<token;
        ss>>rhs;
        
        return *this;
    }
    
    void setDelimiters(const std::string& delimiters);
    
    void clear();
    void rewind();
    
    size_t remaining() const;
    
private:
    std::string mDelimiters;
    std::string mInput;
    size_t mInputItr;
};

#endif /* TOKENIZER_H_ */
