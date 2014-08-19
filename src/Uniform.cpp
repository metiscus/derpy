#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>

#include "Uniform.h"

Uniform::Uniform(Type type, const char* name)
    : mType( type )
    , mName( name )
    , mUniform( -1 )
{
    ;
}

Uniform::~Uniform()
{
    
}

//void Uniform::set( float x )
//{
//    glUniform1f( mUniform, x );
//}

void Uniform::set( int x )
{
    glUniform1i( mUniform, x );
}

void Uniform::set( glm::vec2 v )
{
    glUniform2f( mUniform, v.x, v.y );
}

void Uniform::set( glm::vec3 v )
{
    glUniform3f( mUniform, v.x, v.y, v.z );
}

void Uniform::set( glm::vec4 v )
{
    glUniform4f( mUniform, v.x, v.y, v.z, v.w );
}

void Uniform::set( glm::mat2 m )
{
    glUniformMatrix2fv( mUniform, 1, GL_FALSE, glm::value_ptr(m) );
}

void Uniform::set( glm::mat3 m )
{
    glUniformMatrix3fv( mUniform, 1, GL_FALSE, glm::value_ptr(m) );
}

void Uniform::set( glm::mat4 m )
{
    glUniformMatrix4fv( mUniform, 1, GL_FALSE, glm::value_ptr(m) );
}
