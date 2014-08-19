#ifndef TEXTURE_H_
#define TEXTURE_H_

class Texture
{
public:
    Texture();
    ~Texture();
    
    bool loadFromFile( const char* filename );
    
    void bind();
    
private:
    unsigned int mTexture;
};

#endif /* TEXTURE_H_ */
