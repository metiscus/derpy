#include "Tileset.h"
#include "Logging.h"
#include "Platform.h"

#include <sstream>

namespace map
{

Tileset::Tileset()
    : mFirstGid(0)
    , mLastGid(0)
    , mTileWidth(0)
    , mTileHeight(0)
    , mName("")
{
    ;
}

void Tileset::load(rapidxml::xml_node<> *tsNode)
{
    if (!tsNode)
    {
        Error("(null) node passed in.");
    }
    else
    {
        mName = tsNode->first_attribute("name")->value();

        std::stringstream ss;
        ss << tsNode->first_attribute("tilewidth")->value();
        ss >> mTileWidth;
        ss.clear();

        ss << tsNode->first_attribute("tileheight")->value();
        ss >> mTileHeight;
        ss.clear();

        // gid
        ss << tsNode->first_attribute("firstgid")->value();
        ss >> mFirstGid;
        ss.clear();

        // get the image
        rapidxml::xml_node<> *imageNode = tsNode->first_node("image");
        if (!imageNode)
        {
            Error("Tileset does not include an image element.");
        }
        else
        {
            mImage.filename = imageNode->first_attribute("source")->value();

            ss << imageNode->first_attribute("width")->value();
            ss >> mImage.width;
            ss.clear();

            ss << imageNode->first_attribute("height")->value();
            ss >> mImage.height;
            ss.clear();

            mLastGid = mFirstGid + mImage.width / mTileWidth * (mImage.height / mTileHeight);

            Trace("Loaded tileset '%s' (%u x %u) <%s, %u, %u>.", mName.c_str(),
                  mTileWidth, mTileHeight, mImage.filename.c_str(), mImage.width,
                  mImage.height);
        }
    }
}

void Tileset::getTileIdRange(uint32_t &low, uint32_t &high) const
{
    low = mFirstGid;
    high = mLastGid;
}

bool Tileset::containsTile(uint32_t gid) const
{
    return (gid >= mFirstGid) && (gid <= mLastGid) && (mFirstGid != mLastGid);
}

glm::vec2 Tileset::getTexCoords(uint32_t gid) const
{
    glm::vec2 coords(0., 0.);
    if (!containsTile(gid))
    {
        Error("Requested gid %u is not part of tileset %s.", gid, mName.c_str());
    }
    else
    {
        // compute index
        const uint32_t imageIndex = gid - mFirstGid;

        // get texture params and xy
        const uint32_t imageWidthInTiles = mImage.width / mTileWidth;
        const uint32_t row = imageIndex / imageWidthInTiles;
        const uint32_t column = imageIndex % imageWidthInTiles;

        // compute y coordinate
        glm::vec2 step((float)mTileWidth / (float)mImage.width,
                       (float)mTileHeight / (float)mImage.height);
        coords.x = step.x * column;
        coords.y = 1. - step.y * (row + 1);

        Trace("Tile %u => (%f, %f).", gid, coords.x, coords.y);
    }

    return coords;
}

glm::vec2 Tileset::getTexCoordStep() const
{
    Info("DDD %d %d %d %d", mTileWidth, mImage.width, mTileHeight, mImage.height);
    Info("DDD %f %f", (float)mTileWidth / (float)mImage.width,
         (float)mTileHeight / (float)mImage.height);
    return glm::vec2((float)mTileWidth / (float)mImage.width,
                     (float)mTileHeight / (float)mImage.height);
}

Tileset::Image Tileset::getImage() const
{
    return mImage;
}

std::string Tileset::getName() const
{
    return mName;
}
}
