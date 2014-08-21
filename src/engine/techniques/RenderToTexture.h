#ifndef RENDERTOTEXTURE_H_
#define RENDERTOTEXTURE_H_

#include "Platform.h"
#include "Noncopyable.h"

class Texture;

class RenderToTexture : public noncopyable
{
public:
    enum TextureType {
        Color_RGB,
        Depth_16,
        Depth_24
    };
    
    RenderToTexture( int width, int height, TextureType type );
    ~RenderToTexture();
    
    std::shared_ptr<Texture> getTexture();
    
    void begin();
    void end();

private:
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int mFrameBuffer;
    std::shared_ptr<Texture> mTexture;    
};

#endif /* RENDERTOTEXTURE_H_ */
