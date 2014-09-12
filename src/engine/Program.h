#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Noncopyable.h"
#include "Platform.h"
#include "Uniform.h"
#include <string>
#include <vector>

class Shader;

/**
    \brief Encapsulates an OpenGL shader program
*/
class Program : public noncopyable
{
public:
    Program();
    ~Program();
    
    /**
        \brief Add a shader object to be included in the program
        \param ptr a shader object, it must have already been successfully compiled
        Note: Only objects added to the program before link() is called will be included
    */
    void addShader( std::shared_ptr<Shader> ptr );

    /**
        \brief Adds a uniform to the shader program
        \param ptr the uniform object to be added to the program
        Note: Only objects added to the program before link() is called will be included
    */
    void addUniform( std::shared_ptr<Uniform> ptr );
    
    /**
        \brief Links the shader objects and uniform into a program, sets the link error string
        \return Returns true if the link was successful, false if errors
        Note: Important!, Shaders and Uniforms previously added will be cleared at this point
    */
    bool link();

    /**
        \brief Make the shader program active
    */
    void use();
    
    /**
        \brief Returns the link error string from the most recent link() call
    */
    const std::string getLinkErrors();
    
    /**
        \brief Returns the raw OpenGL program object
    */
    unsigned int getProgram() { return mProgram; }

private:
    unsigned int mProgram;                              //!< OpenGL program
    std::string mLinkErrors;                            //!< Link error string
    std::vector< std::shared_ptr<Shader> > mShaders;    //!< Shaders to be linked in
    std::vector< std::shared_ptr<Uniform> > mUniforms;  //!< Uniforms to be linked in
};

#endif /* PROGRAM_H_ */
