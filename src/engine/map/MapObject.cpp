#include "Logging.h"
#include "MapObject.h"
#include "util/Tokenizer.h"

#include <cstring>
#include <sstream>

namespace map
{

/// MapObject
MapObject::MapObject(ObjectType type, uint32_t x, uint32_t y)
    : mX(x)
    , mY(y)
    , mObjectType(type)
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

bool MapObject::hasProperty(const std::string &name) const
{
    return mProperties.count(name) > 0;
}

std::string MapObject::getProperty(const std::string &name) const
{
    std::string ret;

    PropertyMap::const_iterator itr;
    itr = mProperties.find(name);
    if (itr != mProperties.end())
    {
        ret = itr->second;
    }

    return ret;
}

MapObject *MapObject::load(rapidxml::xml_node<> *objectNode)
{
    MapObject *ret = NULL;

    if (!objectNode)
    {
        Error("(null) node passed in.");
    }
    else
    {
        uint32_t x, y;

        // base members
        std::stringstream ss;
        ss << objectNode->first_attribute("x")->value();
        ss >> x;
        ss.clear();
        ss << objectNode->first_attribute("y")->value();
        ss >> y;
        ss.clear();

        Debug("MapObject <%d, %d>", x, y);

        // fetch rectangle members since rectangle is a special snow-flake
        rapidxml::xml_attribute<> *widthAtt = objectNode->first_attribute("width");
        rapidxml::xml_attribute<> *heightAtt = objectNode->first_attribute("height");
        if (widthAtt && heightAtt)
        {
            uint32_t width, height;

            ss << widthAtt->value();
            ss >> width;
            ss.clear();
            ss << heightAtt->value();
            ss >> height;
            ss.clear();

            ret = new Rectangle(x, y, width, height);
        }
        else // Handle the other non-special cases
        {
            rapidxml::xml_node<> *polygonNode = objectNode->first_node("polygon");
            rapidxml::xml_node<> *polylineNode = objectNode->first_node("polyline");
            rapidxml::xml_attribute<> *pointsNode = NULL;

            if (polygonNode)
            {
                // process polygon
                pointsNode = polygonNode->first_attribute("points");
            }
            else if (polylineNode)
            {
                // process polyline
                pointsNode = polylineNode->first_attribute("points");
            }

            // parse out the points
            if (pointsNode)
            {
                std::vector<glm::ivec2> points;
                Tokenizer pairTok(" ");
                std::string line;
                line = pointsNode->value();
                pairTok << line;
                Trace("Line %s", line.c_str());
                while (pairTok.remaining() > 0)
                {
                    glm::ivec2 vec;
                    std::string pair;

                    pairTok >> pair;
                    Trace("Pair %s", pair.c_str());

                    Tokenizer digitTok(",");
                    digitTok << pair;
                    digitTok >> vec.x;
                    digitTok >> vec.y;

                    points.push_back(vec);
                }

                if (polygonNode)
                {
                    ret = new Polygon(x, y, points);
                }
                else if (polylineNode)
                {
                    ret = new PolyLine(x, y, points);
                }
            }
        }

        if (ret)
        {
            // parse all properties
            rapidxml::xml_node<> *propertiesNode = objectNode->first_node("properties");
            if (propertiesNode)
            {
                for (rapidxml::xml_node<> *itr = propertiesNode->first_node("property");
                     itr; itr = itr->next_sibling("property"))
                {
                    // fetch rectangle members since rectangle is a special snow-flake
                    std::string name = itr->first_attribute("name")->value();
                    std::string value = itr->first_attribute("value")->value();
                    ret->mProperties[name] = value;
                    Trace("property: %s => %s", name.c_str(), value.c_str());
                }
            }

            // technically the type flag is also a property
            char *typeCStr = objectNode->first_attribute("type")->value();
            if (typeCStr)
            {
                Trace("property: %s => %s", "type", typeCStr);
                ret->mProperties["type"] = typeCStr;
            }
        }
    }
    return ret;
}

void MapObject::save(rapidxml::xml_node<> *node)
{
    Fatal("unimplemented.");
}

MapObject::ObjectType MapObject::getObjectType() const
{
    return mObjectType;
}

MapObject::MapObject()
    : mX(0)
    , mY(0)
    , mObjectType(InvalidType)
{
    ;
}

void MapObject::setPosition(uint32_t x, uint32_t y)
{
    mX = x;
    mY = y;
}

/// Rectangle
Rectangle::Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
    : MapObject(MapObject::RectangleType, x, y)
    , mWidth(w)
    , mHeight(h)
{
    Debug("Rectangle <%d, %d> (%d, %d)", x, y, w, h);
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

void Rectangle::save(rapidxml::xml_node<> *node)
{
    MapObject::save(node);
    Fatal("unimplemented.");
}

/// Polygon
Polygon::Polygon(uint32_t x, uint32_t y, std::vector<glm::ivec2> points)
    : MapObject(MapObject::PolygonType, x, y)
    , mPoints(points)
{
    Debug("Polygon <%d, %d>", x, y);
    for (size_t ii = 0; ii < points.size(); ++ii)
    {
        Trace("\t(%d, %d)", points[ii].x, points[ii].y);
    }
}

Polygon::~Polygon()
{
    ;
}

std::vector<glm::ivec2> Polygon::getPoints() const
{
    return mPoints;
}

void Polygon::save(rapidxml::xml_node<> *node)
{
    MapObject::save(node);
    Fatal("unimplemented.");
}

/// PolyLine
PolyLine::PolyLine(uint32_t x, uint32_t y, std::vector<glm::ivec2> points)
    : MapObject(MapObject::PolyLineType, x, y)
    , mPoints(points)
{
    Debug("PolyLine <%d, %d>", x, y);
    for (size_t ii = 0; ii < points.size(); ++ii)
    {
        Trace("\t(%d, %d)", points[ii].x, points[ii].y);
    }
}

PolyLine::~PolyLine()
{
    ;
}

std::vector<glm::ivec2> PolyLine::getPoints() const
{
    return mPoints;
}

void PolyLine::save(rapidxml::xml_node<> *node)
{
    MapObject::save(node);
    Fatal("unimplemented.");
}
}
