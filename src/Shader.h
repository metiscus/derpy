#ifndef SHADER_H_
#define SHADER_H_

#include <string>

class Shader
{
public:
    enum Type {
        Vertex,
        Fragment
    };

    Shader(Type type);
    ~Shader();
    
    void loadFile(const char* filename);
    void setText(const char* text);
    
    bool compile();
        
    const std::string& getCompileErrors();

    friend class Program;
    
protected:
    unsigned int get();

private:
    unsigned int mShader;
    std::string mShaderText;
    std::string mCompileErrors;
};

#endif /* SHADER_H_ */
