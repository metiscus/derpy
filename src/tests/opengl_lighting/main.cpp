#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "Logging.h"
#include "stb_image.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Program.h"
#include "Shader.h"
#include "Sampler.h"
#include "Texture.h"
#include "Mesh.h"

#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

std::shared_ptr<Texture> lightingTex;
float theta = 0.f;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
	glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void animate()
{
    theta += 0.025f;
    float* floats = (float*)lightingTex->getDataRW().getData();
    floats[0] = 3.0f * cos(theta);
    floats[1] = 3.0f * sin(theta);
    floats[8] = 10.0f * cos(2 * 3.14159265 - theta * 0.2);
    floats[9] = 0.5;//5.0f * sin(2 * 3.14159265 - theta);
    theta = fmod(theta, 1 / 0.2 * 2 * 3.14159265);

    lightingTex->dirty();
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
	exit(EXIT_FAILURE);
    }


    glfwWindowHint( GLFW_DEPTH_BITS, 16 );
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
	glfwTerminate();
	exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    // initialize glew
    glewInit();

    glfwSetKeyCallback(window, key_callback);

    MeshRenderer br;

    // make an item for our level
    std::shared_ptr<Mesh> boxMesh (new Mesh());
    VertexList verts;
    verts.push_back(glm::vec3(0.0f, 0.0f, -0.1f));
    verts.push_back(glm::vec3(1.0f, 0.0f, -0.1f));
    verts.push_back(glm::vec3(1.0f, 1.0f, -0.1f));
    verts.push_back(glm::vec3(0.0f, 1.0f, -0.1f));

    TexCoordList texCoords;
    texCoords.push_back(glm::vec2(0.0f, 0.0f));
    texCoords.push_back(glm::vec2(1.0f, 0.0f));
    texCoords.push_back(glm::vec2(1.0f, 1.0f));
    texCoords.push_back(glm::vec2(0.0f, 1.0f));

    IndexList indexList;
    indexList.push_back(0);
    indexList.push_back(1);
    indexList.push_back(2);
    indexList.push_back(0);
    indexList.push_back(2);
    indexList.push_back(3);

    ColorList emptyColorList;
    boxMesh->disableColor();
    boxMesh->enableIndexedDrawing();
    boxMesh->addTriangles(verts, texCoords, emptyColorList, indexList);
    boxMesh->translate( glm::vec3(2.0, 0.0, 0.0) );
    
    // make the ground for the level
    std::shared_ptr<Mesh> levelMesh (new Mesh());
    verts.clear();
    verts.push_back(glm::vec3(-10.0f, -1.0f, 0.0f));
    verts.push_back(glm::vec3(10.0f, -1.0f, 0.0f));
    verts.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
    verts.push_back(glm::vec3(-10.0f, 0.0f, 0.0f));

    texCoords.clear();
    texCoords.push_back(glm::vec2(0.0f, 0.0f));
    texCoords.push_back(glm::vec2(1.0f, 0.0f));
    texCoords.push_back(glm::vec2(1.0f, 0.05f));
    texCoords.push_back(glm::vec2(0.0f, 0.05f));

    levelMesh->disableColor();
    levelMesh->enableIndexedDrawing();
    levelMesh->addTriangles(verts, texCoords, emptyColorList, indexList);
    levelMesh->scale(glm::vec3(1., 0.25, 1.0));

    MeshRenderer mr2;

    // Create the lighting info
    float lightInfo [] = { 3.0, 0.0, 0.0, 3.0, 0.7, 0.0, 0.0, 0,
			   0.0, 6.0, 0.0, 1.0, 0.7, 0.7, 0.7, 0.9 };
    
    TextureData lightingTexData(2, 2, 4, TextureData::Texel_F32, (const unsigned char*)lightInfo);
    
    lightingTex.reset(new Texture());
    lightingTex->setFromData(lightingTexData);

    // Load up the texture
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    Texture wallTexture;
    wallTexture.loadFromFile("data/stonewall.jpg");
    wallTexture.enableGenerateMipmaps();

    Texture crateTexture;
    crateTexture.loadFromFile("data/crate.jpg");
    wallTexture.enableGenerateMipmaps();
    
    Texture boardTexture;
    boardTexture.loadFromFile("data/boards.jpg");
    wallTexture.enableGenerateMipmaps();

static const char* lightingVertexShader =
	"#version 330\n"
	"layout(location=0) in vec3 vertexIn;\n"
	"layout(location=2) in vec2 texCoordIn;\n"
	"out vec2 texCoordOut;\n"
	"out vec3 position;\n"
	"uniform mat4 projectionMat;\n"
	"uniform mat4 viewMat;\n"
	"void main() {\n"
	"gl_Position = projectionMat * viewMat * vec4(vertexIn.xyz, 1.f);\n"
	"position = (viewMat * vec4(vertexIn.xyz, 1.f)).xyz;\n"
	"texCoordOut = texCoordIn;\n"
	"}\n";

static const char* lightingFragmentShader =
	"#version 330\n"
	"uniform sampler2D texSampler;\n"
	"uniform sampler2D lightSampler;\n"
	"in vec2 texCoordOut;\n"
	"in vec3 position;\n"
	"layout(location=0) out vec4 fragColor;\n"
	"void main() {\n"
	"   int numLights = 2;\n"
	"   vec4 color = vec4(0.f, 0.f, 0.f, 0.f);\n"
	"   for(int i=0; i<numLights; ++i) {\n"
	"	vec4 lightPosInfo   = texelFetch(lightSampler, ivec2(0,i), 0);\n"
	"	vec4 lightColorInfo = texelFetch(lightSampler, ivec2(1,i), 0);\n"
	"	float intensity     = clamp(1.0 - (length(position - lightPosInfo.xyz) / lightPosInfo.w), 0, 1);\n"
	//"	intensity = clamp(intensity, 0.0, 1.0 );\n"
	"       color += intensity * 0.8 * vec4(lightColorInfo.xyz, 1.0);\n"
	"   }\n"
	"   color.w = clamp(color.w, 0., 0.94);\n"
	"   vec4 realColor = vec4(texture2D(texSampler, texCoordOut).rgb, 1.0f);\n"
	"   float mixd = color.w; mixd = clamp(mixd, 0., 0.8); \n"
	"   fragColor = mixd * color + mixd * realColor;\n"
	"   //fragColor = vec4(texture(texSampler, texCoordOut).rgb,1.0f)+vec4(texCoordOut.rg, 1.0, 1.0f);\n"
	"}";

    br.setShaderText(Shader::Vertex, lightingVertexShader);
    br.setShaderText(Shader::Fragment, lightingFragmentShader);

    // set up camera
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    br.getCamera()->setView(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
    br.getCamera()->setOrthographic(-10, 10,-5, 5, -1, 1);
    
    mr2.getCamera()->setView(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
    mr2.getCamera()->setOrthographic(-10, 10,-5, 5, -1, 1);
    
    Sampler sampler;
    Sampler lightingSampler;
    
    std::shared_ptr<Uniform> lightingUniform ( new Uniform( Uniform::Uniform_sampler2D, "lightSampler" ) );
    br.addCustomUniform( lightingUniform );

    glClearColor(0., 0., 0., 1.);
    glClearDepth(1.0);
    
    glEnable(GL_BLEND);
    
    wallTexture.setParameter( Texture::Wrap_T, GL_REPEAT );
    wallTexture.setParameter( Texture::Wrap_S, GL_REPEAT );
    wallTexture.setParameter( Texture::MinFilter, GL_LINEAR_MIPMAP_NEAREST );
    wallTexture.setParameter( Texture::MagFilter, GL_LINEAR );
    
    while (!glfwWindowShouldClose(window))
    {
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	br.begin();

	glActiveTexture(GL_TEXTURE0);
	crateTexture.bind();
	br.bindTexture(0);
	sampler.bind(0);

	glActiveTexture(GL_TEXTURE2);
	lightingTex->bind();
	lightingUniform->set(2);
	lightingSampler.bind(2);
	
	//boxMesh->draw();
	boxMesh->translate(glm::vec3(-8.0, 0.0, 0.0));
	br.draw(boxMesh);

	boxMesh->translate(glm::vec3(2.0, 0.0, 0.0));
	br.draw(boxMesh);

	boxMesh->translate(glm::vec3(2.0, 0.0, 0.0));
	br.draw(boxMesh);
		
	boxMesh->translate(glm::vec3(2.0, 0.0, 0.0));
	br.draw(boxMesh);

	boxMesh->translate(glm::vec3(2.0, 0.0, 0.0));
	br.draw(boxMesh);
	
	glActiveTexture(GL_TEXTURE0);
	boardTexture.bind();
	br.bindTexture(0);
	br.draw(levelMesh);

	br.end();

	glfwSwapBuffers(window);
	glfwPollEvents();
	
	animate();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
