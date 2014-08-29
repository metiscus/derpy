#ifndef MAP_H_
#define MAP_H_

#include "Platform.h"

#include <string>
#include <vector>

namespace map 
{

class Layer;
class ObjectGroup;
class Tileset;

class Map
{
public:
    typedef std::vector< std::shared_ptr<Tileset> > TilesetList;
    typedef std::vector< std::shared_ptr<Layer> > LayerList;
    typedef std::vector< std::shared_ptr<ObjectGroup> > ObjectGroupList;
    
    Map();
    ~Map();
    
    bool load(const std::string& filename);
    
    LayerList getLayers();
    ObjectGroupList getObjectGroups();
    TilesetList getTilesets();
    
private:
    LayerList       mLayers;
    ObjectGroupList mObjectGroups;
    TilesetList     mTilesets;
};

}

#endif /* MAP_H_ */
