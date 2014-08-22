#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
	exit(EXIT_FAILURE);
    }

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

    // make a new mesh
    std::shared_ptr<Mesh> mesh (new Mesh());
    VertexList verts;
    verts.push_back(glm::vec3( 0.0f, 0.0f, 0.0f));
    verts.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));
    verts.push_back(glm::vec3( 1.0f, 1.0f, 0.0f));
    verts.push_back(glm::vec3( 0.0f, 1.0f, 0.0f));

    TexCoordList texCoords;
    texCoords.push_back(glm::vec2(0.0f, 0.0f));
    texCoords.push_back(glm::vec2(1.0f, 0.0f));
    texCoords.push_back(glm::vec2(1.0f, 1.0f));
    texCoords.push_back(glm::vec2(0.0f, 1.0f));

    IndexList indexList;
    indexList.push_back( 0 );
    indexList.push_back( 1 );
    indexList.push_back( 2 );
    indexList.push_back( 0 );
    indexList.push_back( 2 );
    indexList.push_back( 3 );

    ColorList emptyColorList;
    mesh->disableColor();
    mesh->enableIndexedDrawing();
    mesh->addTriangles(verts, texCoords, emptyColorList, indexList);

    // Load up the texture
    glEnable(GL_TEXTURE_2D);

    Texture texture;
    texture.loadFromFile("data/crate.jpg");

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    br.getCamera()->setOrthographic(-5, 5, -5, 5, -1, 1);
    
    Sampler sampler;
    bool old = true;
    int counter = 0;

    glClearColor(0., 0., 0., 1.);
    
    while (!glfwWindowShouldClose(window))
    {
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT);

	br.begin();

	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	br.bindTexture(0);
	sampler.bind(0);
	
	mesh->draw();

	br.end();

	glfwSwapBuffers(window);
	glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
