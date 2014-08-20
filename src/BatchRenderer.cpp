#include "BatchRenderer.h"
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

BatchRenderer::BatchRenderer()
{
    _buildShaderProgram();
}

BatchRenderer::~BatchRenderer()
{

}

void BatchRenderer::draw(const Camera& camera)
{
    mProgram->use();
    mProjectionMatrixUniform->set(camera.getProjectionMatrix());
    mViewMatrixUniform->set(camera.getViewMatrix());
}

void BatchRenderer::_buildShaderProgram()
{
    mProgram.reset(new Program());

    const char* defaultVertexShader =
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
    std::shared_ptr<Shader> vertexShader(new Shader(Shader::Vertex));
    vertexShader->setText(defaultVertexShader);
    if(!vertexShader->compile())
    {
	fprintf(stderr, "Shader compile error: %s\n", vertexShader->getCompileErrors().c_str());
    }

    const char* defaultFragmentShader =
	    "#version 330\n"
	    "uniform sampler2D texSampler;\n"
	    "in vec2 texCoordOut;\n"
	    "layout(location=0) out vec4 fragColor;\n"
	    "void main() {\n"
	    "   fragColor = vec4(texture2D(texSampler, texCoordOut).rgb,1.0f);\n"
	    "   //fragColor = vec4(texture(texSampler, texCoordOut).rgb,1.0f)+vec4(texCoordOut.rg, 1.0, 1.0f);\n"
	    "}";
    std::shared_ptr<Shader> fragmentShader(new Shader(Shader::Fragment));
    fragmentShader->setText(defaultFragmentShader);
    if(!fragmentShader->compile())
    {
	fprintf(stderr, "Shader compile error: %s\n", fragmentShader->getCompileErrors().c_str());
    }

    mProjectionMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "projectionMat"));
    mViewMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "viewMat"));
    mSamplerUniform.reset(new Uniform(Uniform::Uniform_sampler2D, "texSampler"));

    mProgram->addUniform(mProjectionMatrixUniform);
    mProgram->addUniform(mViewMatrixUniform);
    mProgram->addUniform(mSamplerUniform);

    mProgram->addShader(vertexShader);
    mProgram->addShader(fragmentShader);
    if(!mProgram->link())
    {
	fprintf(stderr, "Shader compile error: %s\n", mProgram->getLinkErrors().c_str());
    }

    mProgram->use();
    mSamplerUniform->set(0);

    glBindFragDataLocation( mProgram->getProgram(), 0, "fragColor" );
}

void BatchRenderer::bindTexture( int id )
{
    mSamplerUniform->set(0);
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
