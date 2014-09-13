#ifndef MESH_H_
#define MESH_H_

#include "Platform.h"
#include "Noncopyable.h"
#include <glm/glm.hpp>
#include <vector>

class Texture;
class VBO;

typedef std::vector<glm::vec3> VertexList;
typedef std::vector<glm::vec4> ColorList;
typedef std::vector<glm::vec2> TexCoordList;
typedef std::vector<unsigned short> IndexList;

class Mesh : public noncopyable
{
public:
    Mesh();
    ~Mesh();
    
    void draw();
    
    void addTriangles(const VertexList& verts, const TexCoordList& texcoords, const ColorList& colors, const IndexList& indices);
    void addIndices(const IndexList& indices);
    
    bool disableColor();
    bool enableColor();
    
    bool enableIndexedDrawing();
    bool disableIndexedDrawing();
    
    const glm::mat4& getModelMatrix() const;
    void setModelMatrix(const glm::mat4& matrix);
    
    void translate(glm::vec3 vec);
    void rotate(glm::mat3 rot);
    void scale(glm::vec3 scale);
    
    void reset();
    
private:
    std::shared_ptr<VBO> mVBO;
    std::shared_ptr<VBO> mElementBuffer;
    bool mIsBufferDirty;                    //!< When true, the client data has changed and the server data needs updated
    bool mUseColor;                         //!< Pass along color information with the verts
    bool mUseIndexedDrawing;                //!< Draw with drawElements and use the element buffer vs drawArrays
    std::vector<float> mData;               //!< Client side data in the VBO
    IndexList mIndices;                     //!< Client side data in the element array index
    glm::mat4 mModelMatrix;                 //!< Model matrix
    
    VertexList mVertsRaw;                   //!< Verts from the user
    ColorList  mColorsRaw;                  //!< Colors from the user
    TexCoordList mTexCoordsRaw;             //!< Texcoords from the user
    IndexList mIndicesRaw;                  //!< Indicies from the user
    
    //! Uploads the VBO and element buffer to OpenGL
    void _synchronizeDataBuffers();
};

#endif /* MESH_H_ */
