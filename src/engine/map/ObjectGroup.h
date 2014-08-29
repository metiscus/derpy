#ifndef OBJECTGROUP_H_
#define OBJECTGROUP_H_

#include "Platform.h"

#include <map>
#include <string>
#include <vector>
#include <rapidxml.hpp>

namespace map
{

class MapObject;

class ObjectGroup
{
public:
    ObjectGroup();
    ObjectGroup( rapidxml::xml_node<>* node );
    void load( rapidxml::xml_node<>* groupNode );
    //void save( rapidxml::xml_node<>* mapNode );

    typedef std::vector<std::shared_ptr<MapObject> > ObjectList;
    
    ObjectList getObjects();
    size_t getObjectCount() const;
    
private:
    std::string mName;
    ObjectList mObjects;
};

}

#endif /* OBJECTGROUP_H_ */
