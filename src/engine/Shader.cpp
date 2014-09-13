#include <GL/glew.h>
#include <GL/gl.h>

#include "Shader.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

Shader::Shader(Type type)
{
    switch (type)
    {
    case Vertex:
    {
        mShader = glCreateShader(GL_VERTEX_SHADER);
        break;
    }
    case Fragment:
    {
        mShader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    case Geometry:
    {
        mShader = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    }
    default:
    {
        assert(false);
    }
    }
}

Shader::~Shader()
{
    mShaderText = "";
    glDeleteShader(mShader);
}

void Shader::loadFile(const char *filename)
{
    std::string line;
    std::ifstream infile(filename);

    while (infile.is_open() && !infile.eof())
    {
        std::getline(infile, line);
        mShaderText += line + "\n";
    }
    infile.close();
}

void Shader::setText(const char *text)
{
    mShaderText = text;
}

bool Shader::compile()
{
    bool ret = true;
    char *shaderSource = strdup(mShaderText.c_str());
    glShaderSource(mShader, 1, &shaderSource, NULL);
    glCompileShader(mShader);

    GLint shader_ok;
    glGetShaderiv(mShader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok)
    {
        GLint log_length;
        char *log;
        glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &log_length);
        log = (char *)malloc(log_length);
        glGetShaderInfoLog(mShader, log_length, NULL, log);
        // debug
        mCompileErrors = log;
        fprintf(stderr, "%s", log);
        free(log);

        ret = false;
    }
    free(shaderSource);

    return ret;
}

unsigned int Shader::get()
{
    return mShader;
}

const std::string &Shader::getCompileErrors()
{
    return mCompileErrors;
}
