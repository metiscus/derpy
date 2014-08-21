#ifndef UNIFORM_H_
#define UNIFORM_H_

#include <glm/glm.hpp>
#include <string>

class Uniform
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
    Type mType;
    std::string mName;
    int mUniform;

};

#endif /* UNIFORM_H_ */
