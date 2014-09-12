#ifndef UNIFORM_H_
#define UNIFORM_H_

#include <glm/glm.hpp>
#include <string>

#include "Noncopyable.h"

/// Encapsulates an OpenGL uniform
class Uniform : public noncopyable
{
public:
    enum Type {
        Uniform_1f,
        Uniform_2f,
        Uniform_3f,
        Uniform_4f,
        Uniform_mat2,
        Uniform_mat3,
        Uniform_mat4,
        Uniform_sampler2D
    };
    
    /**
        \brief creates a uniform with a type and name
        \param type what type of uniform to create (you can not change this later)
        \param name c-style string containing name of the uniform to create
    */
    Uniform(Type type, const char* name);
    ~Uniform();
    
    //void set( float x );
    void set( glm::vec2 v );
    void set( glm::vec3 v );
    void set( glm::vec4 v );
    void set( glm::mat2 m );
    void set( glm::mat3 m );
    void set( glm::mat4 m );

    void set( int x );
    
    friend class Program;
    
private:
    Type mType;         //!< Type of the uniform
    std::string mName;  //!< Name of the uniform
    int mUniform;       //!< Raw OpenGL object

};

#endif /* UNIFORM_H_ */
