#include <GL/glew.h>
#include <GL/gl.h>

#include "DebugRenderer.h"

#include "Camera.h"
#include "Logging.h"
#include "Program.h"
#include "Shader.h"
#include "Uniform.h"
#include "VBO.h"

enum VertexAttribs
{
    PositionAttr = 0,
    ColorAttr = 1,
    TexCoordAttr = 2
};

static const char *defaultVertexShader = "#version 330\n"
                                         "layout(location=0) in vec3 vertexIn;\n"
                                         "layout(location=1) in vec3 color;\n"
                                         "out vec3 colorOut;\n"
                                         "uniform mat4 projectionMat;\n"
                                         "uniform mat4 viewMat;\n"
                                         "void main() {\n"
                                         "	gl_Position = projectionMat * viewMat * vec4(vertexIn.xyz, 1.f);\n"
                                         "	colorOut = color;\n"
                                         "}\n";

static const char *defaultFragmentShader = "#version 330\n"
                                           "in vec3 colorOut;\n"
                                           "layout(location=0) out vec4 fragColor;\n"
                                           "void main() {\n"
                                           "   fragColor = vec4(colorOut.rgb, 1.f);\n"
                                           //"   fragColor = vec4(1.f, 1.f, 1.f, 1.f);\n"
                                           "}";

DebugRenderer::DebugRenderer()
    : mVBO(new VBO(VBO::ArrayBuffer))
    , mElementBuffer(new VBO(VBO::ElementArrayBuffer))
    , mProgram(new Program())
    , mIsDirty(true)
    , mPrimitiveCount(0)
{
    Trace("Making Renderer");
    std::shared_ptr<Shader> vertexShader(new Shader(Shader::Vertex));
    vertexShader->setText(defaultVertexShader);
    if (!vertexShader->compile())
    {
        Warning("Vertex shader compile error: %s",
                vertexShader->getCompileErrors().c_str());
    }

    std::shared_ptr<Shader> fragmentShader(new Shader(Shader::Fragment));
    fragmentShader->setText(defaultFragmentShader);
    if (!fragmentShader->compile())
    {
        Warning("Fragment shader compile error: %s",
                fragmentShader->getCompileErrors().c_str());
    }

    mProjectionMatrixUniform.reset(
        new Uniform(Uniform::Uniform_mat4, "projectionMat"));
    mViewMatrixUniform.reset(new Uniform(Uniform::Uniform_mat4, "viewMat"));

    mProgram->addUniform(mProjectionMatrixUniform);
    mProgram->addUniform(mViewMatrixUniform);

    //
    mProgram->addShader(vertexShader);
    mProgram->addShader(fragmentShader);
    if (!mProgram->link())
    {
        Warning("Shader program link error: %s", mProgram->getLinkErrors().c_str());
    }

    mProgram->use();

    glBindFragDataLocation(mProgram->getProgram(), 0, "fragColor");

}

DebugRenderer::~DebugRenderer()
{
	;
}

void DebugRenderer::addLines(std::vector<glm::vec3> points, glm::vec3 color)
{
	Trace("addLineStrip");
  mIsDirty = true; 
	
	uint32_t lastIndex = mVerts.size();

	for(uint32_t newPointCount=0; newPointCount<points.size(); ++newPointCount)
	{
		mVerts.push_back(points[newPointCount]);
    Trace("<%f %f %f>", points[newPointCount].x, points[newPointCount].y, points[newPointCount].z);
		mColors.push_back(color);

		// write primitive indicies
		mIndices.push_back(lastIndex + newPointCount);
	}

	++mPrimitiveCount;
}

void DebugRenderer::draw(std::shared_ptr<Camera> camera)
{
    if(mIsDirty)
    {
      Trace("building data etc");
	    std::vector<float> data;
	    data.reserve(mVerts.size() * 2 * 3);

	    for(uint32_t ii=0; ii<mVerts.size(); ++ii)
	    {
	    	data.push_back(mVerts[ii].x);
	    	data.push_back(mVerts[ii].y);
	    	data.push_back(mVerts[ii].z);
	    	   
	    	data.push_back(mColors[ii].x);
	    	data.push_back(mColors[ii].y);
	    	data.push_back(mColors[ii].z);
	    }
      
        mVBO->bind();
        mElementBuffer->bind();
	    mVBO->setData(&data[0], data.size() * sizeof(float), VBO::DynamicDraw);
	    mElementBuffer->setData(&mIndices[0], mIndices.size() * sizeof(short), VBO::DynamicDraw);

	    mIsDirty = false;	    
    }

    mProgram->use();
    mProjectionMatrixUniform->set(camera->getProjectionMatrix());
    mViewMatrixUniform->set(camera->getViewMatrix());

    mVBO->bind();
    mElementBuffer->bind();

    glEnableVertexAttribArray(PositionAttr);
    glEnableVertexAttribArray(ColorAttr);

    // note: buffer format is pos pos pos color color color
    int stride = sizeof(float) * (3 + 3);
    glVertexAttribPointer(PositionAttr, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(ColorAttr, 3, GL_FLOAT, GL_FALSE, stride,
                          (void *)(sizeof(float) * 3));

    glDrawElements(GL_LINES, mPrimitiveCount, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(ColorAttr);
    glDisableVertexAttribArray(PositionAttr);

    mVBO->unbind();
    mElementBuffer->unbind();
}