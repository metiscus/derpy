#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Noncopyable.h"
#include "Platform.h"

#include <vector>

/**
    \brief Data used to create a Texture (usually used for data textures)
*/
class TextureData
{
public:
    enum TexelType {
        Texel_Invalid = 0,  //!< Invalid texel format
        Texel_U8 = 1,       //!< Unsigned byte 0-255
        Texel_F32 = 4       //!< 32 bit float 
    };
    
    /// Creates an empty texture data object
    TextureData();

    /**
        \brief Creates a TextureData object and populates the fields
        \param width width in texels
        \param height height in texels
        \param channels number of color components for each texel (1=Grayscale, 2=Grayscale with Alpha, 3=RGB, 4=RGBA)
        \param data pointer to a buffer which contains at least width * height * channels * size of either unsigned byte or float depending on the type parameter
    */
    TextureData(int width, int height, int channels, TexelType type, const unsigned char* data);
    ~TextureData();
    
    /**
        \brief Resizes an existing TextureData object and populates the fields
        \param width width in texels
        \param height height in texels
        \param channels number of color components for each texel (1=Grayscale, 2=Grayscale with Alpha, 3=RGB, 4=RGBA)
        \param data pointer to a buffer which contains at least width * height * channels * size of either unsigned byte or float depending on the type parameter
    */
    void resize(int width, int height, int channels, TexelType type, const unsigned char* data);
    
    /// Returns the width in texels
    int getWidth() const;
    /// Returns the height in texels
    int getHeight() const;
    /// Returns the number of color components per texel
    int getChannels() const;
    /// Returns the texel type
    TexelType getType() const;
        
    /// Returns a raw pointer to the internal data
    unsigned char* getData();
    /// Returns a const pointer to the internal data
    const unsigned char* getData() const;
    /// Allows indexed operations on the internal data by texel
    unsigned char* operator[](const unsigned int& index);
    /**
        \brief Allows indexed operations on the internal data by texel
        \param x - texels in s direction
        \param y - texels in t direction
    */
    unsigned char* operator()(const unsigned int& x, const unsigned int& y);
    
    /// Some image formats require scanline inversion, this function inverts the scanlines
    void flipImageVertical();
    
private:
    int mWidth;                         //!< Width in texels
    int mHeight;                        //!< Height in texels
    int mChannels;                      //!< Color component count 
    TexelType mType;                    //!< Type of each color component
    std::vector<unsigned char> mData;   //!< Raw data (protected with a std::vector)
};

/// Encapsulation of an OpenGL texture with convenience methods for loading some images
class Texture : public noncopyable
{
public:
    Texture();
    ~Texture();
    
    /**
        \brief load a texture from an image file
        \param filename c-string containing the name of an image file to load
    */
    bool loadFromFile(const char* filename);

    /**
        \brief load a texture from a TextureData object
        \param data a reference to the data from which to populate the object
    */
    bool setFromData(const TextureData& data);
    
    /// Returns a reference to the internal image data, sets dirty bit
    TextureData& getDataRW();

    /// Returns a reference to the internal image data, does not set the dirty big
    const TextureData& getDataRO() const;
    
    /// Binds the texture to the currently active texture unit
    void bind();
    
    /// Describes various texture parameters that can be set
    enum Parameter {
        MinFilter,      //!< Minification filter
        MagFilter,      //!< Magnification filter
        Wrap_S,         //!< Texture wrap in s direction (x)
        Wrap_T,         //!< Texture wrap in t direction (y)
    };
    
    /**
        \brief Sets a texture parameter for this texture
        \param param which parameter to set the value for
        \param value an OpenGL enum to set for that parameter
    */
    void setParameter( Parameter param, unsigned int value );
    
    /// Returns the width in texels
    int getWidth() const;
    /// Returns the height in texels
    int getHeight() const;
    
    /// Checks for equality between textures
    bool operator==(const Texture& rhs) const;
    
    /**
        \brief Enable hardware mipmap generation
        Important! Texture that are not power of 2 may segfault. YMMV
    */
    void enableGenerateMipmaps();

    /**
        \brief Disable hardware mipmap generation
    */
    void disableGenerateMipmaps();
    
    /// Returns the raw OpenGL object
    unsigned int getObject();
    
    /// Sets the dirty bit which causes the texture to get reuploaded.
    /// This is expensive, so only call this when you have changed the data
    /// through a pointer or some other naughty activity.
    void dirty();
    
private:
    unsigned int mTexture;      //!< OpenGL texture object
    bool mIsDirty;              //!< Flag that indicates if the texture data needs reuploaded
    bool mGenerateMipmaps;      //!< Flag that controls hardware mipmap generation
    TextureData mData;          //!< Raw data for the texture (to be uploaded when dirty)
    
    void _updateImageData();    //!< Handles uploading dirty data to OpenGL
};

#endif /* TEXTURE_H_ */
