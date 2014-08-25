#include "Logging.h"
#include "Mesh.h"
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
    : mIsDirty(true)
    , mIsRendering(false) 
    , mCamera(new Camera())    
{
    mVertexShaderSource   = defaultVertexShader;
    mFragmentShaderSource = defaultFragmentShader;
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::setShaderText( Shader::Type type, const std::string& text )
{
    switch(type)
    {
	case Shader::Vertex:
	{
	    mIsDirty = true;
	    mVertexShaderSource = text;
	    break;
	}
	case Shader::Fragment:
	{
	    mIsDirty = true;
	    mFragmentShaderSource = text;
	    break;
	}
	default:
	{
	    Warning("invalid type specified.");
	}
    }    
}

void MeshRenderer::begin()
{
    if(mIsRendering)
    {
	Error("end() must be called before calling begin() a second time.");
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
	Error("begin() must be called before calling end() a second time.");
    }
    mIsRendering = false;
}

void MeshRenderer::draw(std::shared_ptr<Mesh> mesh)
{
    glm::mat4 origViewMatrix = mCamera->getViewMatrix();
    glm::mat4 viewMatrix = origViewMatrix * mesh->getModelMatrix();
    mViewMatrixUniform->set(viewMatrix);
    mesh->draw();
    mViewMatrixUniform->set(origViewMatrix);
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
    mSamplerUniform->set(id);
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
	Warning("Vertex shader compile error: %s", vertexShader->getCompileErrors().c_str());
    }

    std::shared_ptr<Shader> fragmentShader(new Shader(Shader::Fragment));
    fragmentShader->setText(mFragmentShaderSource.c_str());
    if(!fragmentShader->compile())
    {
	Warning("Fragment shader compile error: %s", fragmentShader->getCompileErrors().c_str());
    }

    mProjectionMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "projectionMat"));
    mViewMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "viewMat"));
    mSamplerUniform.reset(new Uniform(Uniform::Uniform_sampler2D, "texSampler"));

    mProgram->addUniform(mProjectionMatrixUniform);
    mProgram->addUniform(mViewMatrixUniform);
    mProgram->addUniform(mSamplerUniform);
    
    // 
    for(unsigned int ii=0; ii<mCustomUniforms.size(); ++ii)
    {
	mProgram->addUniform(mCustomUniforms[ii]);
    }
    
    //
    mProgram->addShader(vertexShader);
    mProgram->addShader(fragmentShader);
    if(!mProgram->link())
    {
	Warning("Shader program link error: %s",  mProgram->getLinkErrors().c_str());
    }

    mProgram->use();
    mSamplerUniform->set(0);

    glBindFragDataLocation( mProgram->getProgram(), 0, "fragColor" );
}
