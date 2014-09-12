#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdio>
#include <cstdlib>

#include "Logging.h"
#include "Program.h"
#include "Shader.h"
#include "Uniform.h"

Program::Program()
{
    mProgram = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(mProgram);
}
    
void Program::addShader( std::shared_ptr<Shader> ptr )
{
    mShaders.push_back(ptr);
}

void Program::addUniform( std::shared_ptr<Uniform> uniform )
{
    mUniforms.push_back(uniform);
}

bool Program::link()
{   
    std::vector< std::shared_ptr<Shader> >::iterator itr;
    for(  itr = mShaders.begin(); itr!=mShaders.end(); ++itr )
    {
        glAttachShader(mProgram, (*itr)->get());
    }
    
    glLinkProgram(mProgram);
    
    GLint program_ok;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &program_ok);
    if (!program_ok) 
    {
        GLint log_length;
        char *log;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &log_length);
        log = (char*)malloc(log_length);
        glGetProgramInfoLog(mProgram, log_length, NULL, log);
        // debug
        mLinkErrors = log;
        Error("%s", log);
        free(log);
        return false;
    }
    
    // for each uniform attached, find it in the linked program
    for(unsigned int ii=0; ii<mUniforms.size(); ++ii)
    {
        mUniforms[ii]->mUniform = glGetUniformLocation(mProgram, mUniforms[ii]->mName.c_str());
        Debug("Found %s at %d", mUniforms[ii]->mName.c_str(), mUniforms[ii]->mUniform);
    }
    
    //DEBUG: we shouldn't keep these around, so let's free them.
    mUniforms.clear();
    mShaders.clear();

    return true;
}

void Program::use()
{
    glUseProgram(mProgram);    
}
    
const std::string Program::getLinkErrors()
{
    return mLinkErrors;
}
