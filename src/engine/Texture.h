#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Noncopyable.h"

class Texture : public noncopyable
{
public:
    Texture();
    ~Texture();
    
    bool loadFromFile( const char* filename );
    
    void bind();
    
    int getWidth() const;
    int getHeight() const;
    
    bool operator==( const Texture& rhs ) const;
    
    unsigned int getObject();
    
private:
    int mWidth;
    int mHeight;
    unsigned int mTexture;
};

#endif /* TEXTURE_H_ */
