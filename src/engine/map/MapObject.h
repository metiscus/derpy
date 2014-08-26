#ifndef MAPOBJECT_H_
#define MAPOBJECT_H_

#include "Platform.h"
#include <string>
#include <map>
#include <vector>

#include <rapidxml.hpp>

#include <glm/glm.hpp>

typedef std::map<std::string, std::string> PropertyMap;

class MapObject 
{
public:  
    enum Type {
        Rectangle,
        Polygon,
        PolyLine
    };
  
    MapObject(Type type, const std::string& name, uint32_t x, uint32_t y);
    virtual ~MapObject();
    
    uint32_t getX() const;
    uint32_t getY() const;
    
    bool hasProperty(const std::string& name) const;
    std::string getProperty(const std::string& name) const;
    
    static MapObject* load( rapidxml::xml_node<>* objectNode );
    virtual void save( rapidxml::xml_node<>* node ) = 0;
    
    Type getType() const;
    
private:
    std::string mName;
    uint32_t mX;
    uint32_t mY;
    PropertyMap mProperties;
    Type mType;
};

class Rectangle : public MapObject
{
public:
    Rectangle(const std::string& name, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    virtual ~Rectangle();
    
    uint32_t getW() const;
    uint32_t getH() const;
    
    virtual void save( rapidxml::xml_node<>* node );
    
private:
    uint32_t mWidth;
    uint32_t mHeight;
};

class Polygon : public MapObject
{
public:
    Polygon(const std::string& name, uint32_t x, uint32_t y, std::vector<glm::ivec3> points);
    virtual ~Polygon();
    
    std::vector<glm::ivec3> getPoints() const;

    virtual void save( rapidxml::xml_node<>* node );

private:
    std::vector<glm::ivec3> mPoints;
};

class PolyLine : public MapObject
{
public:
    PolyLine(const std::string& name, uint32_t x, uint32_t y, std::vector<glm::ivec3> points);
    virtual ~PolyLine();
    
    std::vector<glm::ivec3> getPoints() const;

    virtual void save( rapidxml::xml_node<>* node );

private:
    std::vector<glm::ivec3> mPoints;
};

#endif /* MAPOBJECT_H_ */
