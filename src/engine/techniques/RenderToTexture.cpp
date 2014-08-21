#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdio>

#include "Platform.h"
#include "Texture.h"
#include "RenderToTexture.h"

// technique implemented from 
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

RenderToTexture::RenderToTexture(int width, int height, TextureType type)
    : mTexture(new Texture())
    , mWidth(width)
    , mHeight(height)
{
    glGenFramebuffers(1, &mFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

    if(type==Depth_16 || type==Depth_24)
    {
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer); 
    }

    // build the texture
    mTexture->bind();
    switch(type)
    {
        case Color_RGB:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        break;
        
        case Depth_16:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        break;
        
        case Depth_24:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // attach the buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture->getObject(), 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "[RenderToTexture::RenderToTexture()] failed to create the requested renderbuffer.\n");
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderToTexture::~RenderToTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &mFrameBuffer);
}

std::shared_ptr<Texture> RenderToTexture::getTexture()
{
    return mTexture;
}

void RenderToTexture::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
}

void RenderToTexture::end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
