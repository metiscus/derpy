#ifndef TILESET_H_
#define TILESET_H_

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <rapidxml.hpp>
#include "Platform.h"

namespace map 
{

class Tileset
{
public:
    struct Image
    {
        Image() {
            filename = "";
            width = 0;
            height = 0;
        }
        std::string filename;
        uint32_t width;
        uint32_t height;
    };

    Tileset();
    
    void load(rapidxml::xml_node<>* tsNode);
    
    void getTileIdRange(uint32_t &low, uint32_t &high) const;
    bool containsTile(uint32_t gid) const;
    glm::vec2 getTexCoords(uint32_t gid) const;
    glm::vec2 getTexCoordStep() const;
    Image getImage() const;
    
    std::string getName() const;

    //\TODO: expose tile width and height

private:
    uint32_t mFirstGid;
    uint32_t mLastGid;
    uint32_t mTileWidth;
    uint32_t mTileHeight;
    std::string mName;
    
    Image mImage;  
};

}

#endif /* TILESET_H_ */
