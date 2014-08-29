#include "Map.h"
#include "Layer.h"
#include "Logging.h"
#include "ObjectGroup.h"
#include "Tileset.h"
#include <rapidxml_utils.hpp>

namespace map
{

Map::Map()
{
    ;
}

Map::~Map()
{
    ;
}

bool Map::load(const std::string& filename)
{
    rapidxml::file<> mapfile(filename.c_str());
    
    rapidxml::xml_document<> doc;
    doc.parse<0>(mapfile.data());
    
    // get map node
    rapidxml::xml_node<> *map = doc.first_node("map");
    
    if(!map)
    {
        Error("%s did not contain a valid map.", filename.c_str());
        return false;
    }
    
    // parse out tile sets
    for(rapidxml::xml_node<> *itr = map->first_node("tileset"); itr; itr = itr->next_sibling("tileset"))
    {
        std::shared_ptr<Tileset> tsPtr(new Tileset());
        tsPtr->load(itr);
        mTilesets.push_back(tsPtr);
    }
    
    // parse out layers
    for(rapidxml::xml_node<> *itr = map->first_node("layer"); itr; itr = itr->next_sibling("layer"))
    {
        std::shared_ptr<Layer> layerPtr(new Layer(itr));
        if(layerPtr.get()!=NULL)
        {
            mLayers.push_back(layerPtr);
        }
    }

    // parse out object groups
    for(rapidxml::xml_node<> *itr = map->first_node("objectgroup"); itr; itr = itr->next_sibling("objectgroup"))
    {
        std::shared_ptr<ObjectGroup> objgrpPtr(new ObjectGroup(itr));
        if(objgrpPtr.get()!=NULL)
        {
            mObjectGroups.push_back(objgrpPtr);
        }
    }
    
    return true;
}

Map::LayerList Map::getLayers()
{
    return mLayers;
}

Map::ObjectGroupList Map::getObjectGroups()
{
    return mObjectGroups;
}

Map::TilesetList Map::getTilesets()
{
    return mTilesets;
}

}
