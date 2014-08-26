#include "Logging.h"
#include "MapObject.h"


/// MapObject
MapObject::MapObject(Type type, const std::string& name, uint32_t x, uint32_t y)
    : mName(name)
    , mX(x)
    , mY(y)
    , mType(type)
{
    ;
}

MapObject::~MapObject()
{
    ;
}

uint32_t MapObject::getX() const
{
    return mX;
}

uint32_t MapObject::getY() const
{
    return mY;
}

bool MapObject::hasProperty(const std::string& name) const
{
    return mProperties.count(name)>0;
}

std::string MapObject::getProperty(const std::string& name) const
{
    std::string ret;
    
    PropertyMap::const_iterator itr;
    itr = mProperties.find(name);
    if(itr != mProperties.end())
    {
        ret = itr->second;
    }
    
    return ret;
}

MapObject* MapObject::load( rapidxml::xml_node<>* objectNode )
{
    Fatal("unimplemented.");
    return NULL;
}

void MapObject::save( rapidxml::xml_node<>* node )
{
    Fatal("unimplemented.");
}

MapObject::Type MapObject::getType() const
{
    return mType;
}


/// Rectangle
Rectangle::Rectangle(const std::string& name, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
    : MapObject(MapObject::Rectangle, name, x, y)
    , mWidth(w)
    , mHeight(h)
{
    ;
}

Rectangle::~Rectangle()
{
    ;
}

uint32_t Rectangle::getW() const
{
    return mWidth;
}

uint32_t Rectangle::getH() const
{
    return mHeight;
}

void Rectangle::save( rapidxml::xml_node<>* node )
{
    MapObject::save(node);
    Fatal("unimplemented.");
}

/// Polygon
Polygon::Polygon(const std::string& name, uint32_t x, uint32_t y, std::vector<glm::ivec3> points)
    : MapObject(MapObject::Polygon, name, x, y)
    , mPoints(points)
{
    ;
}

Polygon::~Polygon()
{
    ;
}

std::vector<glm::ivec3> Polygon::getPoints() const
{
    return mPoints;
}

void Polygon::save( rapidxml::xml_node<>* node )
{
    MapObject::save(node);
    Fatal("unimplemented.");
}

/// PolyLine
PolyLine::PolyLine(const std::string& name, uint32_t x, uint32_t y, std::vector<glm::ivec3> points)
    : MapObject(MapObject::PolyLine, name, x, y)
    , mPoints(points)
{
    ;
}

PolyLine::~PolyLine()
{
    ;
}
    
std::vector<glm::ivec3> PolyLine::getPoints() const
{
    return mPoints;
}

void PolyLine::save( rapidxml::xml_node<>* node )
{
    MapObject::save(node);
    Fatal("unimplemented.");
}
