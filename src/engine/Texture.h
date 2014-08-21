#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Noncopyable.h"
#include "Platform.h"

#include <vector>

class TextureData
{
public:
    enum TexelType {
        Texel_Invalid = 0,
        Texel_U8 = 1,
        Texel_F32 = 4
    };
    
    TextureData();
    TextureData(int width, int height, int channels, TexelType type, const unsigned char* data);
    ~TextureData();
    
    void resize(int width, int height, int channels, TexelType type, const unsigned char* data);
    
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    TexelType getType() const;
    
    unsigned char* getData();
    const unsigned char* getData() const;
    unsigned char* operator[](const unsigned int& index);
    unsigned char* operator()(const unsigned int& x, const unsigned int& y);
    
private:
    int mWidth;
    int mHeight;
    int mChannels;
    TexelType mType;
    std::vector<unsigned char> mData;
};

class Texture : public noncopyable
{
public:
    Texture();
    ~Texture();
    
    bool loadFromFile(const char* filename);
    bool setFromData(const TextureData& data);
    
    void bind();
    
    int getWidth() const;
    int getHeight() const;
    
    bool operator==(const Texture& rhs) const;
    
    void enableGenerateMipmaps();
    void disabeGenerateMipmaps();
    
    unsigned int getObject();
    
private:
    unsigned int mTexture;
    bool mIsDirty;
    bool mGenerateMipmaps;
    TextureData mData;
    
    void _updateImageData();
};

#endif /* TEXTURE_H_ */
