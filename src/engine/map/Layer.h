#ifndef LAYER_H_
#define LAYER_H_

#include "Platform.h"

#include <string>
#include <vector>

#include <rapidxml.hpp>

typedef uint32_t TileId;

class Layer
{
public:
    Layer();
    Layer( rapidxml::xml_node<>* node );
    
    void load( rapidxml::xml_node<>* layerNode );
    void save( rapidxml::xml_node<>* mapNode );
    
    void resize(uint32_t width, uint32_t height, TileId defaultValue);
    
    const std::string& getName() const;
    void setName( const std::string& name );
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    const TileId& get(uint32_t x, uint32_t y);
    void set(uint32_t x, uint32_t y, const TileId& id);
    
private:
    uint32_t mWidth;
    uint32_t mHeight;
    std::string mName;
    std::vector<TileId> mData;
};

#endif /* LAYER_H_ */
