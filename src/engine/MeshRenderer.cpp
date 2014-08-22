#include "MeshRenderer.h"
#include "Camera.h"
#include "Shader.h"
#include "Program.h"
#include "Uniform.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include "Camera.h"

static const char* defaultVertexShader =
	"#version 330\n"
	"layout(location=0) in vec3 vertexIn;\n"
	"layout(location=2) in vec2 texCoordIn;\n"
	"out vec2 texCoordOut;\n"
	"uniform mat4 projectionMat;\n"
	"uniform mat4 viewMat;\n"
	"void main() {\n"
	"gl_Position = projectionMat * viewMat * vec4(vertexIn.xyz, 1.f);\n"
	"texCoordOut = texCoordIn;\n"
	"}\n";

static const char* defaultFragmentShader =
	"#version 330\n"
	"uniform sampler2D texSampler;\n"
	"in vec2 texCoordOut;\n"
	"layout(location=0) out vec4 fragColor;\n"
	"void main() {\n"
	"   fragColor = vec4(texture2D(texSampler, texCoordOut).rgb,1.0f);\n"
	"   //fragColor = vec4(texture(texSampler, texCoordOut).rgb,1.0f)+vec4(texCoordOut.rg, 1.0, 1.0f);\n"
	"}";	

MeshRenderer::MeshRenderer()
    : mCamera(new Camera())
    , mIsRendering(false)
    , mIsDirty(true)
{
    mVertexShaderSource = defaultVertexShader;
    mFragmentShaderSource = defaultFragmentShader;
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::setShaderText( Shader::Type type, const std::string& text )
{
    
    
}

void MeshRenderer::begin()
{
    if(mIsRendering)
    {
	fprintf(stderr, "[MeshRenderer::begin] MeshRenderer::end() must be called before calling begin() again.\n");
    }
    
    if(mIsDirty)
    {
	mIsDirty = false;
	_buildShaderProgram();
    }
    
    mProgram->use();
    mProjectionMatrixUniform->set(mCamera->getProjectionMatrix());
    mViewMatrixUniform->set(mCamera->getViewMatrix());
    mIsRendering = true;
}

void MeshRenderer::end()
{
    //\TODO: Implement cleaning up our state
    if(!mIsRendering)
    {
	fprintf(stderr, "[MeshRenderer::end] MeshRenderer::begin() must be called before calling end().\n");
    }
    mIsRendering = false;
}

std::shared_ptr<Camera> MeshRenderer::getCamera()
{
    return mCamera;
}

void MeshRenderer::setCamera(std::shared_ptr<Camera> camera)
{
    mCamera = camera;
}

void MeshRenderer::bindTexture(int id)
{
    mSamplerUniform->set(0);
}

void MeshRenderer::addCustomUniform(std::shared_ptr<Uniform> uniform)
{
    mCustomUniforms.push_back(uniform);
}

void MeshRenderer::_buildShaderProgram()
{
    mProgram.reset(new Program());

    std::shared_ptr<Shader> vertexShader(new Shader(Shader::Vertex));
    vertexShader->setText(mVertexShaderSource.c_str());
    if(!vertexShader->compile())
    {
	fprintf(stderr, "[MeshRenderer::_buildShaderProgram] Shader compile error: %s\n", vertexShader->getCompileErrors().c_str());
    }

    std::shared_ptr<Shader> fragmentShader(new Shader(Shader::Fragment));
    fragmentShader->setText(mFragmentShaderSource.c_str());
    if(!fragmentShader->compile())
    {
	fprintf(stderr, "[MeshRenderer::_buildShaderProgram] Shader compile error: %s\n", fragmentShader->getCompileErrors().c_str());
    }

    mProjectionMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "projectionMat"));
    mViewMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "viewMat"));
    mSamplerUniform.reset(new Uniform(Uniform::Uniform_sampler2D, "texSampler"));

    mProgram->addUniform(mProjectionMatrixUniform);
    mProgram->addUniform(mViewMatrixUniform);
    mProgram->addUniform(mSamplerUniform);
    
    // 
    for(int ii=0; ii<mCustomUniforms.size(); ++ii)
    {
	mProgram->addUniform(mCustomUniforms[ii]);
    }
    
    //
    mProgram->addShader(vertexShader);
    mProgram->addShader(fragmentShader);
    if(!mProgram->link())
    {
	fprintf(stderr, "[MeshRenderer::_buildShaderProgram] Shader compile error: %s\n", mProgram->getLinkErrors().c_str());
    }

    mProgram->use();
    mSamplerUniform->set(0);

    glBindFragDataLocation( mProgram->getProgram(), 0, "fragColor" );
}

static void show_info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char*)malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}
