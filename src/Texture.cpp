#include <GL/glew.h>
#include <GL/gl.h>

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
    
bool Texture::loadFromFile( const char* filename )
{
    int c;
    const GLubyte* textureData = stbi_load(filename, &mWidth, &mHeight, &c, 4);
    if( textureData != NULL )
    {
        fprintf(stderr, "[Texture::loadFromFile] loaded %s (%d x %d) with %d channels.\n", filename, mWidth, mHeight, c);
        fprintf(stderr, "[Texture::loadFromFile] forcing RGBA\n");
        //GLuint type = (c==4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

        stbi_image_free((void*)textureData);
	
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool Texture::operator==( const Texture& rhs ) const
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
