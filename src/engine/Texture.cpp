#include <GL/glew.h>
#include <GL/gl.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "Logging.h"
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
    Debug("width=%d height=%d channels=%d type=%d data=%p.", width, height, channels, type, data);
    
    resize(width, height, channels, type, data);
}

TextureData::~TextureData()
{
    ;
}

void TextureData::resize(int width, int height, int channels, TexelType type, const unsigned char* data)
{
    Debug("width=%d height=%d channels=%d type=%d data=%p.", width, height, channels, type, data);
    
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
    const size_t texelWidth = (size_t)mType;
    return &mData[index*texelWidth*mChannels];
}

unsigned char* TextureData::operator()(const unsigned int& x, const unsigned int& y)
{
    // compute texel width in bytes
    const size_t texelWidth = (size_t)mType;
    return &(mData[texelWidth*(y * mWidth * mChannels + x)]);
}

void TextureData::flipImageVertical()
{
    // compute texel width in bytes
    const size_t texelWidth = (size_t)mType;
    const size_t lineWidth  = texelWidth * mChannels * mWidth;
    std::vector<unsigned char> scanline;
    scanline.resize(lineWidth);
    for(int ii=0; ii<=mHeight / 2; ++ii)
    {
        unsigned char* ptr_to   = &mData[ii * lineWidth];
        unsigned char* ptr_from = &mData[(mHeight - 1 - ii) * lineWidth];
        scanline.assign(ptr_to, ptr_to+lineWidth);
        memcpy(ptr_to, ptr_from, lineWidth);
        memcpy(ptr_from, &scanline[0], lineWidth);
    }
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

bool Texture::loadFromFile(const char* filename)
{
    if(!filename)
    {
        Fatal("null filename.");
        return false;
    }
    
    int c, width, height;   
    const GLubyte* loadedData = stbi_load(filename, &width, &height, &c, 4);
    if( loadedData != NULL )
    {
        mIsDirty = true;
        
        Debug("loaded %s (%d x %d) with %d channels.", filename, width, height, c);
        Debug("forcing RGBA.");
        
        // copy data to internal buffer and release
        mData.resize(width, height, 4, TextureData::Texel_U8, loadedData);
        stbi_image_free((void*)loadedData);
                
        // some file extensions require flipping
        std::string extension = filename;
        size_t itr = extension.rfind(".");
        if(itr!=std::string::npos)
        {
            extension = extension.substr(itr + 1, extension.length()-itr);
            Debug("file is of type %s.", extension.c_str());
            if(extension != "bmp")
            {
                //textureVFlip(mData.getData(), width, height);
                mData.flipImageVertical();
                Debug("inverting scanlines.");
            }
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        return false;
    }
    
    return true;
}

bool Texture::setFromData(const TextureData& data)
{
    mIsDirty = true;
    mData = data;
    return true;
}

TextureData& Texture::getDataRW()
{
    mIsDirty = true;    // automatically mark as dirty
    return mData;
}

const TextureData& Texture::getDataRO() const
{
    return mData;    
}

void Texture::dirty()
{
    mIsDirty = true;
}

bool Texture::operator==(const Texture& rhs) const
{
    return mTexture == rhs.mTexture;
}

int Texture::getWidth() const
{
    return mData.getWidth();
}

int Texture::getHeight() const
{
    return mData.getHeight();
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

void Texture::disableGenerateMipmaps()
{
    mGenerateMipmaps = false;
}

void Texture::_updateImageData()
{
    if(mIsDirty)
    {
        mIsDirty = false;
        Info("texture %d has dirty data, refreshing.", mTexture);
       
        glBindTexture(GL_TEXTURE_2D, mTexture);
        
        GLenum texelType = GL_RGBA;
        
        switch(mData.getChannels())
        {
            case 1: texelType = GL_RED; break;
            case 2: texelType = GL_RG; break;
            case 3: texelType = GL_RGB; break;
            case 4: texelType = GL_RGBA; break;
            default: 
            {
                Warning("invalid channel count, ignoring data."); 
                return; 
                break;
            }
        }
        
        GLenum dataType;
        switch(mData.getType())
        {
            case TextureData::Texel_U8: dataType = GL_UNSIGNED_BYTE; break;
            case TextureData::Texel_F32: dataType = GL_FLOAT; break;
            
            case TextureData::Texel_Invalid: // intentional fall-through
            default: 
            {
                Warning("invalid channel size, ignoring data."); 
                return; 
                break;
            }
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, texelType, mData.getWidth(), mData.getHeight(), 0, texelType, dataType, mData.getData());
	
        if(mGenerateMipmaps)
        {
            Info("generating mipmaps.");
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
}
