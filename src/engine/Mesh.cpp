#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>

#include "Logging.h"
#include "Mesh.h"

enum VertexAttribs
{
    PositionAttr = 0,
    ColorAttr = 1,
    TexCoordAttr = 2
};

Mesh::Mesh()
    : mIsBufferDirty(false)
    , mUseColor(true)
    , mUseIndexedDrawing(false)
{
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mElementBuffer);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mElementBuffer);
}

void Mesh::_synchronizeDataBuffers()
{
    if (mIsBufferDirty)
    {
        Debug("rebuilding VBO due to dirty buffer flag: %d.", mData.size());
        mIsBufferDirty = false;
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, mData.size() * sizeof(float), &mData[0],
                     GL_DYNAMIC_DRAW);
        if (mUseIndexedDrawing)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(short),
                         &mIndices[0], GL_DYNAMIC_DRAW);
        }
    }
}

void Mesh::draw()
{
    _synchronizeDataBuffers();

    if (mUseIndexedDrawing)
    {
        // Info("using indexed drawing");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
    }
    else
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glEnableVertexAttribArray(PositionAttr);
    glEnableVertexAttribArray(TexCoordAttr);

    // note: buffer format is pos pos pos tex tex color color color color
    int stride = sizeof(float) * (3 + 2 + (mUseColor ? 4 : 0));
    glVertexAttribPointer(PositionAttr, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(TexCoordAttr, 2, GL_FLOAT, GL_FALSE, stride,
                          (void *)(sizeof(float) * 3));
    if (mUseColor)
    {
        Info("using color vertex attributes");
        glEnableVertexAttribArray(ColorAttr);
        glVertexAttribPointer(ColorAttr, 4, GL_FLOAT, GL_FALSE, stride,
                              (void *)(sizeof(float) * 5));
    }

    if (mUseIndexedDrawing)
    {
        unsigned int triangleCount = (mUseColor) ? mData.size() / (3 + 2 + 4) : mData.size() / (3 + 2);
        glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_SHORT, 0);
    }
    else
    {
        unsigned int indexCount = mVertsRaw.size();
        glDrawArrays(GL_TRIANGLES, 0, indexCount);
    }

    glDisableVertexAttribArray(TexCoordAttr);
    glDisableVertexAttribArray(PositionAttr);
    if (mUseColor)
    {
        glDisableVertexAttribArray(ColorAttr);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (mUseIndexedDrawing)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::addTriangles(const VertexList &verts, const TexCoordList &texcoords,
                        const ColorList &colors, const IndexList &indices)
{
    bool isValidData = true;
    unsigned int numTriangles = 0;

    if (indices.size() == 0 && verts.size() == 0)
    {
        return;
    }

    if (mUseIndexedDrawing)
    {
        numTriangles = indices.size() / 3;
        if (3 * numTriangles != indices.size())
        {
            Error("passed in index list not multiple of three.");
            isValidData = false;
        }
    }
    else
    { // using draw arrays
        numTriangles = verts.size() / 3;
        if (3 * numTriangles != verts.size())
        {
            Error("passed in vertex list not multiple of three.");
            isValidData = false;
        }
    }

    if (texcoords.size() != verts.size())
    {
        Error("passed in texture coordinate list that does not match.");
        isValidData = false;
    }

    if ((mUseColor && colors.size() != verts.size()) || (!mUseColor && colors.size() > 0))
    {
        Error("passed in color coordinate list that does not match the number of "
              "verts.");
        isValidData = false;
    }

    // valid data needs packed into the buffer
    // buffer format is v v v t t {c c c c}
    if (isValidData)
    {
        mIsBufferDirty = true;

        //
        mVertsRaw.insert(mVertsRaw.end(), verts.begin(), verts.end());
        mColorsRaw.insert(mColorsRaw.end(), colors.begin(), colors.end());
        mTexCoordsRaw.insert(mTexCoordsRaw.end(), texcoords.begin(),
                             texcoords.end());
        mIndicesRaw.insert(mIndicesRaw.end(), indices.begin(), indices.end());

        mIndices.insert(mIndices.end(), indices.begin(), indices.end());

        for (unsigned int triangle = 0; triangle < 3 * numTriangles; ++triangle)
        {
            mData.push_back(verts[triangle].x);
            mData.push_back(verts[triangle].y);
            mData.push_back(verts[triangle].z);
            mData.push_back(texcoords[triangle].x);
            mData.push_back(texcoords[triangle].y);

            // Info("<%f %f %f> (%f, %f)", verts[triangle].x, verts[triangle].y,
            // verts[triangle].z, texcoords[triangle].x, texcoords[triangle].y);
            // for(unsigned int ii=0; ii<mData.size(); ++ii)
            //{
            //    Info("%f", mData[ii]);
            //}

            if (mUseColor)
            {
                mData.push_back(colors[triangle].x);
                mData.push_back(colors[triangle].y);
                mData.push_back(colors[triangle].z);
                mData.push_back(colors[triangle].w);
            }
        }
    }
}

void Mesh::addIndices(const IndexList &indices)
{
    if (mUseIndexedDrawing)
    {
        unsigned int numTriangles = indices.size() / 3;
        if (3 * numTriangles != indices.size())
        {
            Error("passed in index list not multiple of three.");
        }
        else
        {
            mIsBufferDirty = true;
            mIndicesRaw.insert(mIndicesRaw.end(), indices.begin(), indices.end());
            mIndices.insert(mIndices.end(), indices.begin(), indices.end());
        }
    }
    else
    {
        Error("tried adding index data when not in indexed draw mode.");
    }
}

const glm::mat4 &Mesh::getModelMatrix() const
{
    return mModelMatrix;
}

void Mesh::setModelMatrix(const glm::mat4 &matrix)
{
    mModelMatrix = matrix;
}

void Mesh::translate(glm::vec3 vec)
{
    mModelMatrix = glm::translate(mModelMatrix, vec);
}

void Mesh::rotate(glm::mat3 rot)
{
    mModelMatrix *= glm::mat4(rot);
}

void Mesh::scale(glm::vec3 scale)
{
    mModelMatrix = glm::scale(mModelMatrix, scale);
}

bool Mesh::disableColor()
{
    mUseColor = false;
    mIsBufferDirty = true;
    ColorList emptyColorList;

    Debug("color attributes disabled.");

    if (mUseIndexedDrawing)
    {
        addTriangles(mVertsRaw, mTexCoordsRaw, emptyColorList, mIndicesRaw);
    }
    else
    {
        IndexList emptyIndexList;
        addTriangles(mVertsRaw, mTexCoordsRaw, emptyColorList, emptyIndexList);
    }
    return true;
}

bool Mesh::enableColor()
{
    mUseColor = true;
    mIsBufferDirty = true;

    Debug("color attributes enabled.");

    if (mUseIndexedDrawing)
    {
        addTriangles(mVertsRaw, mTexCoordsRaw, mColorsRaw, mIndicesRaw);
    }
    else
    {
        IndexList emptyIndexList;
        addTriangles(mVertsRaw, mTexCoordsRaw, mColorsRaw, emptyIndexList);
    }
    return true;
}

bool Mesh::enableIndexedDrawing()
{
    Debug("indexed drawing enabled.");
    mUseIndexedDrawing = true;
    return true;
}

bool Mesh::disableIndexedDrawing()
{
    Debug("indexed drawing disabled.");
    mUseIndexedDrawing = false;
    return true;
}

void Mesh::reset()
{
    mIsBufferDirty = true;

    mVertsRaw.clear();
    mTexCoordsRaw.clear();
    mColorsRaw.clear();
    mIndicesRaw.clear();
    mIndices.clear();
    mData.clear();
}
