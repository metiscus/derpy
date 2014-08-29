#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "map/Map.h"
#include "map/Tileset.h"

#include "Logging.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Platform.h"
#include "Texture.h"

#include <map>
#include <string>

int32_t gWidth = 640;
int32_t gHeight = 480;
GLFWwindow* gWindow = NULL;

const std::string gDataPath = "data/map/";

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
    if (!glfwInit()) {
        return 1;
    }
    
    // make the window
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    
    gWindow = glfwCreateWindow(gWidth, gHeight, "Map Rendering", NULL, NULL);
    if (!gWindow)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(gWindow);
    glfwSetKeyCallback(gWindow, key_callback);
    glewInit();

    glViewport(0, 0, gWidth, gHeight);
    glClearColor(0., 0., 0., 1.);
    
    map::Map map;
    if(!map.load(gDataPath + "untitled.tmx"))
    {
        Error("Unable to load map. Exiting.");
        exit(1);
    }
    
    // Load required textures for the map
    std::map<std::string, std::shared_ptr<Texture> > textureMap;
    map::Map::TilesetList tilesets = map.getTilesets();
    map::Map::TilesetList::iterator itr;
    for(itr=tilesets.begin(); itr!=tilesets.end(); ++itr)
    {
        std::string textureFilename = (*itr)->getImage().filename;
        
        if(textureMap.count(textureFilename)==0)
        {
            Debug("Loading texture %s.", (gDataPath + textureFilename).c_str());
            std::shared_ptr<Texture> texture ( new Texture() );
            if(!texture->loadFromFile((gDataPath + textureFilename).c_str()))
            {
                Error("Unable to load a required texture. Exiting.");
                exit(1);
            }
        }
    }
    
    MeshRenderer mapRenderer;

    // Generate the geometry for the map
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
    
        
    while (!glfwWindowShouldClose(gWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(gWindow);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(gWindow);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
