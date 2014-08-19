#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Platform.h"
#include "Uniform.h"
#include <string>
#include <vector>

class Shader;
//class Uniform;

class Program
{
public:
    Program();
    ~Program();
    
    void addShader( std::shared_ptr<Shader> ptr );
    void addUniform( std::shared_ptr<Uniform> ptr );
    bool link();
    void use();
    
    const std::string getLinkErrors();
    
    unsigned int getProgram() { return mProgram; }

private:
    unsigned int mProgram;
    std::string mLinkErrors;
    std::vector< std::shared_ptr<Shader> > mShaders;
    std::vector< std::shared_ptr<Uniform> > mUniforms;
};

#endif /* PROGRAM_H_ */
