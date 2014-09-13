#include "ObjectGroup.h"
#include "MapObject.h"
#include "Logging.h"

namespace map
{

ObjectGroup::ObjectGroup()
{
}

ObjectGroup::ObjectGroup(rapidxml::xml_node<> *node)
{
    load(node);
}

void ObjectGroup::load(rapidxml::xml_node<> *groupNode)
{
    if (!groupNode)
    {
        Error("(null) node passed in.");
    }
    else
    {
        for (rapidxml::xml_node<> *itr = groupNode->first_node("object"); itr;
             itr = itr->next_sibling())
        {
            Info(itr->value());
            MapObject *pObj = MapObject::load(itr);
            if (pObj)
            {
                std::shared_ptr<MapObject> ptr;
                ptr.reset(pObj);
                mObjects.push_back(ptr);
            }
        }
    }
}
// void ObjectGroup::save( rapidxml::xml_node<>* mapNode );

ObjectGroup::ObjectList ObjectGroup::getObjects()
{
    return mObjects;
}

size_t ObjectGroup::getObjectCount() const
{
    return mObjects.size();
}
}
