#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "map/Map.h"
#include "map/Layer.h"
#include "map/Tileset.h"
#include "map/ObjectGroup.h"
#include "map/MapObject.h"

#include "Camera.h"
#include "DebugRenderer.h"
#include "Logging.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Platform.h"
#include "Sampler.h"
#include "Texture.h"

#include <map>
#include <string>

#include <Box2D/Box2D.h>

int32_t gWidth = 640;
int32_t gHeight = 480;
GLFWwindow *gWindow = NULL;

const std::string gDataPath = "data/map/";

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);

int main(void)
{
    if (!glfwInit())
    {
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

    map::Map map;
    if (!map.load(gDataPath + "untitled.tmx"))
    {
        Error("Unable to load map. Exiting.");
        exit(1);
    }

    // Load required textures for the map
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
    map::Map::TilesetList tilesets = map.getTilesets();
    map::Map::TilesetList::iterator itr;
    std::shared_ptr<Texture> theTexture;
    for (itr = tilesets.begin(); itr != tilesets.end(); ++itr)
    {
        std::string textureFilename = (*itr)->getImage().filename;

        if (textureMap.count(textureFilename) == 0)
        {
            Debug("Loading texture %s.", (gDataPath + textureFilename).c_str());
            std::shared_ptr<Texture> texture(new Texture());
            if (!texture->loadFromFile((gDataPath + textureFilename).c_str()))
            {
                Error("Unable to load a required texture. Exiting.");
                exit(1);
            }
            textureMap[textureFilename] = texture;
            theTexture = texture;
        }
    }

    ///\Note: We assume 1 layer, with 1 texture right now
    MeshRenderer mapRenderer;

    map::Map::LayerList layers = map.getLayers();
    std::shared_ptr<map::Layer> layer = layers[0];

    std::shared_ptr<Mesh> boxMesh(new Mesh());
    VertexList verts;
    TexCoordList texCoords;

    IndexList indexList;

    glm::vec2 texOffset = tilesets[0]->getTexCoordStep();

    const float texStepU = texOffset.x;
    const float texStepV = texOffset.y;

    glm::vec3 offset((float)layer->getHeight(), (float)layer->getHeight(), 0.f);
    int32_t vertCount = 0;
    for (uint32_t height = 0; height < layer->getHeight(); ++height)
    {
        for (uint32_t width = 0; width < layer->getWidth(); ++width)
        {
            // Generate the geometry for the map
            const uint32_t tileGid = layer->get(width, height);

            Info("(%d, %d) : %d", width, height, tileGid);

            if (tilesets[0]->containsTile(tileGid))
            {
                glm::vec2 baseCoord = tilesets[0]->getTexCoords(tileGid);

                verts.push_back(offset + glm::vec3(0.0f, 0.0f, 0.1f));
                verts.push_back(offset + glm::vec3(1.0f, 0.0f, 0.1f));
                verts.push_back(offset + glm::vec3(1.0f, 1.0f, 0.1f));
                verts.push_back(offset + glm::vec3(0.0f, 1.0f, 0.1f));

                texCoords.push_back(baseCoord + glm::vec2(0.0f, 0.0f));
                texCoords.push_back(baseCoord + glm::vec2(texStepU, 0.0f));
                texCoords.push_back(baseCoord + glm::vec2(texStepU, texStepV));
                texCoords.push_back(baseCoord + glm::vec2(0.0f, texStepV));

                indexList.push_back(vertCount + 0);
                indexList.push_back(vertCount + 1);
                indexList.push_back(vertCount + 2);
                indexList.push_back(vertCount + 0);
                indexList.push_back(vertCount + 2);
                indexList.push_back(vertCount + 3);

                vertCount += 4;
            }

            offset.x -= 1.f;
        }

        offset.x = (float)layer->getHeight();
        offset.y -= 1.f;
    }

    ///
    /// begin physics
    ///
    const char *ObjectTypeName[] = { "InvalidObjectType", "RectangleObjectType",
                                     "PolygonObjectType", "PolylineObjectType" };


    b2Vec2 gravity(0.f, -10.f);
    b2World physics_world(gravity);
    
    // debug render
    std::shared_ptr<DebugRenderer> debugRenderer(new DebugRenderer());

    std::vector<b2Body *> physicsBodies;

    float scaleFactor = 32.f;

    map::Map::ObjectGroupList objectGroups = map.getObjectGroups();
    map::Map::ObjectGroupList::iterator grpItr;
    for (grpItr = objectGroups.begin(); grpItr != objectGroups.end(); ++grpItr)
    {
        std::shared_ptr<map::ObjectGroup> objects(*(grpItr));
        map::ObjectGroup::ObjectList objectList = objects->getObjects();
        map::ObjectGroup::ObjectList::iterator objItr;
        for (objItr = objectList.begin(); objItr != objectList.end(); ++objItr)
        {
            Trace("object %s\n", ObjectTypeName[(*objItr)->getObjectType()]);

            if ((*objItr)->getObjectType() == 1)
            {
                map::Rectangle *rectPtr = dynamic_cast<map::Rectangle *>((*objItr).get());

                b2BodyDef boxDef;
                if (rectPtr->hasProperty("type") && rectPtr->getProperty("type") == "Terrain")
                {
                    Trace("Creating static body.");
                    boxDef.type = b2_staticBody;
                }
                else
                {
                    Trace("Creating dynamic body.");
                }

                //\Note: these need converted by /32?
                //HACK!! flipping due to coordinate issues
                float x, y, width, height;
                x = rectPtr->getX() / 64.f - 1;
                y = rectPtr->getY() / 64.f - 1;
                width = rectPtr->getW() / 64.f; 
                height = rectPtr->getH() / 64.f;

                boxDef.position = b2Vec2(x, y);
                b2Body *boxBody = physics_world.CreateBody(&boxDef);
                physicsBodies.push_back(boxBody);

                b2PolygonShape boxShape;
                boxShape.SetAsBox(scaleFactor * width, scaleFactor * height);
                boxBody->CreateFixture(&boxShape, 0.0f);

                // debug properties
                //// TESTTESTTEST
                std::vector<glm::vec3> debugBox;
                offset = scaleFactor * glm::vec3(layer->getWidth(), layer->getHeight(), 0.0);
                debugBox.push_back(offset - scaleFactor * glm::vec3(x, y, 0.f));
                debugBox.push_back(offset - scaleFactor * glm::vec3(x + width, y, 0.f));

                debugBox.push_back(offset - scaleFactor * glm::vec3(x + width, y, 0.f));
                debugBox.push_back(offset - scaleFactor * glm::vec3(x + width, y + height, 0.f));

                debugBox.push_back(offset - scaleFactor * glm::vec3(x + width, y + height, 0.f));
                debugBox.push_back(offset - scaleFactor * glm::vec3(x, y + height, 0.f));

                debugBox.push_back(offset - scaleFactor * glm::vec3(x, y + height, 0.f));
                debugBox.push_back(offset - scaleFactor * glm::vec3(x, y, 0.f));

                debugRenderer->addLines(debugBox, glm::vec3(0.f, 0.f, 1.f));
            }
        }
    }

    ///
    /// rendering etc...
    ///
    ColorList emptyColorList;
    boxMesh->disableColor();
    boxMesh->enableIndexedDrawing();
    boxMesh->addTriangles(verts, texCoords, emptyColorList, indexList);

    mapRenderer.getCamera()->setView(glm::vec3(0.0, 0.0, 0.0),
                                     glm::vec3(0.0, 0.0, -1.0),
                                     glm::vec3(0.0, 1.0, 0.0));
    mapRenderer.getCamera()->setOrthographic(0, gWidth, 0, gHeight, -1, 1);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0., 0., 0.112f, 1.);
    glClearDepth(1.0);

    glEnable(GL_BLEND);

    theTexture->setParameter(Texture::Wrap_T, GL_REPEAT);
    theTexture->setParameter(Texture::Wrap_S, GL_REPEAT);
    theTexture->setParameter(Texture::MinFilter, GL_LINEAR_MIPMAP_NEAREST);
    theTexture->setParameter(Texture::MagFilter, GL_LINEAR);

    Sampler sampler;

    boxMesh->scale(glm::vec3(scaleFactor, scaleFactor, 1.f));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //build crate
    glm::vec3 cratePos(10.f, 20.f, 0.f);
    cratePos *= scaleFactor;

    b2BodyDef crateDef;
    crateDef.type = b2_dynamicBody;

    crateDef.position = b2Vec2(cratePos.x, cratePos.y);
    b2Body *crateBody = physics_world.CreateBody(&crateDef);
    physicsBodies.push_back(crateBody);

    b2PolygonShape crateShape;
    crateShape.SetAsBox(5, 5);
    crateBody->CreateFixture(&crateShape, 1.0f);

    while (!glfwWindowShouldClose(gWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mapRenderer.begin();

        glActiveTexture(GL_TEXTURE0);
        theTexture->bind();
        mapRenderer.bindTexture(0);
        sampler.bind(0);

        mapRenderer.draw(boxMesh);

        mapRenderer.end();

        b2Vec2 pPos = crateBody->GetPosition();
        glm::vec3 newPos (pPos.x, pPos.y, 0.f);

        std::vector<glm::vec3> lines;
        lines.push_back(cratePos);
        lines.push_back(newPos);
        cratePos = newPos;
        debugRenderer->addLines(lines, glm::vec3(1.f, 1.f, 1.f));

        debugRenderer->draw(mapRenderer.getCamera());

        physics_world.Step(1.0f / 60.0f, 6, 2);

        glfwSwapBuffers(gWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(gWindow);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
