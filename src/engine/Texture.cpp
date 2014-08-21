#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "stb_image.h"
#include "Texture.h"

Texture::Texture()
{
   glGenTextures( 1, &mTexture );
}

Texture::~Texture()
{
    glDeleteTextures( 1, &mTexture );
}

//Important: assumes pixel size is 4
static inline void textureVFlip(char* buffer, size_t linePixels, size_t lineCount)
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
    
    int c;   
    const GLubyte* loadedData = stbi_load(filename, &mWidth, &mHeight, &c, 4);
    char *textureData = (char*)malloc(mWidth*mHeight*4);
    if( textureData != NULL && loadedData != NULL )
    {
        fprintf(stderr, "[Texture::loadFromFile] loaded %s (%d x %d) with %d channels.\n", filename, mWidth, mHeight, c);
        fprintf(stderr, "[Texture::loadFromFile] forcing RGBA\n");
        
        memcpy(textureData, loadedData, mWidth*mHeight*4);
        
        // some file extensions require flipping
        std::string extension = filename;
        size_t itr = extension.rfind(".");
        if(itr!=std::string::npos)
        {
            extension = extension.substr(itr + 1, extension.length()-itr);
            fprintf(stderr, "[Texture::loadFromFile] file is of type %s\n", extension.c_str());
            if(extension != "bmp")
            {
                textureVFlip(textureData, mWidth, mHeight);
                fprintf(stderr, "[Texture::loadFromFile] inverting scanlines.\n");
            }
        }
        
        //GLuint type = (c==4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    stbi_image_free((void*)loadedData);
    free(textureData);
}

bool Texture::operator==(const Texture& rhs) const
{
    return mTexture == rhs.mTexture;
}

int Texture::getWidth() const
{
    return mWidth;
}

int Texture::getHeight() const
{
    return mHeight;
}
    
void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
}
