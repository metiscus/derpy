#include <GL/glew.h>
#include <GL/gl.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "stb_image.h"
#include "Texture.h"

/********************
 * class TextureData
 ********************/

TextureData::TextureData()
    : mWidth(0)
    , mHeight(0)
    , mChannels(0)
    , mType(Texel_Invalid)
{
    ;
}
 
TextureData::TextureData(int width, int height, int channels, TexelType type, const unsigned char* data)
    : mWidth(width)
    , mHeight(height)
    , mChannels(channels)
    , mType(type)
{
    resize(width, height, channels, type, data);
}

TextureData::~TextureData()
{
    ;
}

void TextureData::resize(int width, int height, int channels, TexelType type, const unsigned char* data)
{
    assert(width>0 && height>0 && channels>0 && data && type != TextureData::Texel_Invalid);
    mWidth = width;
    mHeight = height;
    mChannels = channels;
    mType = type;
    
    // clear existing data
    mData.clear();
    
    // compute texel width in bytes
    size_t texelWidth = (int)type;
    mData.insert(mData.begin(), data, data + (width*height*channels*texelWidth));
}

int TextureData::getWidth() const
{
    return mWidth;
}

int TextureData::getHeight() const
{
    return mHeight;
}

int TextureData::getChannels() const
{
    return mChannels;
}

TextureData::TexelType TextureData::getType() const
{
    return mType;
}

unsigned char* TextureData::getData()
{
    return &mData[0];
}

const unsigned char* TextureData::getData() const 
{
    return &mData[0];
}

unsigned char* TextureData::operator[](const unsigned int& index)
{
    // compute texel width in bytes
    size_t texelWidth = (int)mType;
    return &mData[index*texelWidth*mChannels];
}

unsigned char* TextureData::operator()(const unsigned int& x, const unsigned int& y)
{
    // compute texel width in bytes
    size_t texelWidth = (int)mType;
    return &(mData[texelWidth*(y * mWidth * mChannels + x)]);
}

/********************
 * class Texture
 ********************/
Texture::Texture()
    : mIsDirty(false)
    , mGenerateMipmaps(true)
{
   glGenTextures(1, &mTexture);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTexture);
}

//Important: assumes pixel size is 4
static inline void textureVFlip(unsigned char* buffer, size_t linePixels, size_t lineCount)
{
    size_t pixelCount = linePixels*lineCount;
    int *pixBuff = new int [pixelCount];
    for(size_t head=0, tail=pixelCount-1; tail !=0; ++head, --tail)
    {
        pixBuff[head] = ((int*)buffer)[tail];
    }
    
    memcpy(buffer, pixBuff, pixelCount*sizeof(int));
    delete [] pixBuff;
}

bool Texture::loadFromFile(const char* filename)
{
    if(!filename)
    {
        fprintf(stderr, "[Texture::loadFromFile] invalid filename\n");
        return false;
    }
    
    int c, width, height;   
    const GLubyte* loadedData = stbi_load(filename, &width, &height, &c, 4);
    if( loadedData != NULL )
    {
        mIsDirty = true;
        
        fprintf(stderr, "[Texture::loadFromFile] loaded %s (%d x %d) with %d channels.\n", filename, width, height, c);
        fprintf(stderr, "[Texture::loadFromFile] forcing RGBA\n");
        
        // copy data to internal buffer and release
        mData.resize(width, height, 4, TextureData::Texel_U8, loadedData);
        stbi_image_free((void*)loadedData);
                
        // some file extensions require flipping
        std::string extension = filename;
        size_t itr = extension.rfind(".");
        if(itr!=std::string::npos)
        {
            extension = extension.substr(itr + 1, extension.length()-itr);
            fprintf(stderr, "[Texture::loadFromFile] file is of type %s\n", extension.c_str());
            if(extension != "bmp")
            {
                textureVFlip(mData.getData(), width, height);
                fprintf(stderr, "[Texture::loadFromFile] inverting scanlines.\n");
            }
        }
        
        //GLuint type = (c==4) ? GL_RGBA : GL_RGB;
        //glBindTexture(GL_TEXTURE_2D, mTexture);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	
        //glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool Texture::setFromData(const TextureData& data)
{
    mIsDirty = true;
    mData = data;
}

bool Texture::operator==(const Texture& rhs) const
{
    return mTexture == rhs.mTexture;
}

int Texture::getWidth() const
{
    mData.getWidth();
}

int Texture::getHeight() const
{
    mData.getHeight();
}

unsigned int Texture::getObject()
{
    return mTexture;
}
    
void Texture::bind()
{
    _updateImageData();
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::enableGenerateMipmaps()
{
    mGenerateMipmaps = true;
}

void Texture::disabeGenerateMipmaps()
{
    mGenerateMipmaps = false;
}

void Texture::_updateImageData()
{
    if(mIsDirty)
    {
        mIsDirty = false;
        fprintf(stderr, "[Texture::_updateImageData] texture %d has dirty data, refreshing.\n", mTexture);
       
        glBindTexture(GL_TEXTURE_2D, mTexture);
        
        GLenum texelType = GL_RGBA;
        
        switch(mData.getChannels())
        {
            case 1: texelType = GL_RED; break;
            case 2: texelType = GL_RG; break;
            case 3: texelType = GL_RGB; break;
            case 4: texelType = GL_RGBA; break;
            default: fprintf(stderr, "Texture::_updateImageData] invalid channel count, ignoring data.\n"); mIsDirty = false; return; break;
        }
        
        GLenum dataType;
        switch(mData.getType())
        {
            case TextureData::Texel_U8: dataType = GL_UNSIGNED_BYTE; break;
            case TextureData::Texel_F32: dataType = GL_FLOAT; break;
            
            case TextureData::Texel_Invalid: // intentional fall-through
            default: fprintf(stderr, "Texture::_updateImageData] invalid channel size, ignoring data.\n"); mIsDirty = false; return; break;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, texelType, mData.getWidth(), mData.getHeight(), 0, texelType, dataType, mData.getData());
	
        if(mGenerateMipmaps)
        {
            fprintf(stderr, "[Texture::_updateImageData] generating mipmaps.\n", mTexture);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
}
