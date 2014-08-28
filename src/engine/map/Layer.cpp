#include "Layer.h"
#include "Logging.h"

#include <fstream>
#include <sstream>

Layer::Layer()
    : mWidth(0)
    , mHeight(0)
    , mName("unnamed layer")
{
    ;
}

Layer::Layer( rapidxml::xml_node<>* node )
{
    load(node);
}

void Layer::load( rapidxml::xml_node<>* layerNode )
{
    // parse tag attributes
    mName = layerNode->first_attribute("name")->value();
    
    std::stringstream ss;
    ss << layerNode->first_attribute("width")->value();
    ss >> mWidth;
    ss.clear();
    
    ss << layerNode->first_attribute("height")->value();
    ss >> mHeight;
    ss.clear();
    
    Debug("layer %s (%dx%d).", mName.c_str(), mWidth, mHeight);
    
    // parse data child
    rapidxml::xml_node<> *dataNode = layerNode->first_node("data");
    if(!dataNode)
    {
        Error("unable to find data node for map layer.");
    }
    else {
        // check for encoding types (some are unsupported)
        std::string encodingType = dataNode->first_attribute("encoding")->value();
        if( encodingType == "csv" )
        {
            std::string data = dataNode->value();
            ss<<data;
            
            std::string field;
            while( std::getline( ss, field, ',' ) )
            {
                std::stringstream conv(field);
                TileId tileId;
                conv>>tileId;
                mData.push_back(tileId);
            }
        }
        else 
        {
            Error("data is encoded using unsupported format %s.", encodingType.c_str());
        }
    }
}

void Layer::save( rapidxml::xml_node<>* mapNode )
{
    Fatal("unimplemented.");
}

void Layer::resize(uint32_t width, uint32_t height, TileId defaultValue)
{
    mWidth = width;
    mHeight = height;
    mData.clear();
    mData.resize( width, height );
    for(uint32_t ii=0; ii<width*height; ++ii)
    {
        mData[ii] = defaultValue;
    }
}

const std::string& Layer::getName() const
{
    return mName;
}

void Layer::setName( const std::string& name )
{
    mName = name;
}

uint32_t Layer::getWidth() const
{
    return mWidth;
}

uint32_t Layer::getHeight() const
{
    return mHeight;
}

const TileId& Layer::get(uint32_t x, uint32_t y)
{
    return mData[x + y * mWidth];
}

void Layer::set(uint32_t x, uint32_t y, const TileId& id)
{
    mData[x + y * mWidth] = id;
}
