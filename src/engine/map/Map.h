#ifndef MAP_H_
#define MAP_H_

#include "Platform.h"

#include <string>
#include <vector>

class Layer;
class ObjectGroup;

class Map
{
public:
    typedef std::vector< std::shared_ptr<Layer> > LayerList;
    typedef std::vector< std::shared_ptr<ObjectGroup> > ObjectGroupList;
    
    Map();
    ~Map();
    
    void load(const std::string& filename);
    
    LayerList getLayers();
    ObjectGroupList getObjectGroups();
    
private:
    LayerList       mLayers;
    ObjectGroupList mObjectGroups;
};

#endif /* MAP_H_ */
