#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include "Noncopyable.h"

/**
    \brief Encapsulates an OpenGL shader
*/
class Shader : public noncopyable
{
public:
    enum Type {
        Vertex,     //!< Vertex shader
        Fragment,   //!< Fragment shader
        Geometry    //!< Geometry shader
    };

    /**
        \brief Creates a new shader object
        \param type The type of shader to create, you can not change this after you create it
    */    
    Shader(Type type);
    ~Shader();
    
    /**
        \brief Sets the text of the shader from a file
        \param filename A c-style string that contains the name of a file
    */
    void loadFile(const char* filename);
    
    /**
        \brief Sets the text of the shader from a string
        \param text A c-style string that contains the text for the shader
    */
    void setText(const char* text);
    
    /// Compiles the shader text and sets the compile error string
    bool compile();
        
    /// Returns the compile errors from OpenGL
    const std::string& getCompileErrors();

    friend class Program;
    
protected:
    /// Returns the raw OpenGL shader object
    unsigned int get();

private:
    unsigned int mShader;       //!< OpenGL shader object
    std::string mShaderText;    //!< Text of the shader
    std::string mCompileErrors; //!< Text of any compile errors
};

#endif /* SHADER_H_ */
