#include "Map.h"
#include "Layer.h"
#include "Logging.h"
#include "ObjectGroup.h"
#include <rapidxml_utils.hpp>

Map::Map()
{
    ;
}

Map::~Map()
{
    ;
}

void Map::load(const std::string& filename)
{
    rapidxml::file<> mapfile(filename.c_str());
    
    rapidxml::xml_document<> doc;
    doc.parse<0>(mapfile.data());
    
    // get map node
    rapidxml::xml_node<> *map = doc.first_node("map");
    
    if(!map)
    {
        Error("%s did not contain a valid map.", filename.c_str());
        return;
    }
    
    // parse out tile sets
    ///\TODO
    ;
    
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
}

Map::LayerList Map::getLayers()
{
    return mLayers;
}

Map::ObjectGroupList Map::getObjectGroups()
{
    return mObjectGroups;
}
