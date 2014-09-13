#include <GL/glew.h>
#include <GL/gl.h>

#include "VBO.h"

VBO::VBO(BindMode mode)
    : mMode(mode)
{
    glGenBuffers(1, &mVBO);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &mVBO);
}

void VBO::setData(void *data, size_t dataSize, UsageType usage)
{
    int32_t mode = _bindToGL(mMode);
    glBindBuffer(mode, mVBO);
    glBufferData(mode, dataSize, data, _usageToGL(usage));
}

void VBO::setDataOffset(int offset, void *data, size_t dataSize)
{
    int32_t mode = _bindToGL(mMode);
    glBindBuffer(mode, mVBO);
    glBufferSubData(mode, offset, dataSize, data);
}

void *VBO::mapBuffer(MappingType type)
{
    int32_t mode = _bindToGL(mMode);
    glBindBuffer(mode, mVBO);
    return glMapBuffer(mode, _mappingToGL(type));
}

void VBO::unmapBuffer()
{
    glUnmapBuffer(_bindToGL(mMode));
}

void VBO::bind()
{
    glBindBuffer(_bindToGL(mMode), mVBO);
}

void VBO::unbind()
{
    glBindBuffer(_bindToGL(mMode), 0);
}

int32_t VBO::_bindToGL(BindMode mode)
{
    switch (mode)
    {
    case ArrayBuffer:
    {
        return GL_ARRAY_BUFFER;
        break;
    }
    case ElementArrayBuffer:
    {
        return GL_ELEMENT_ARRAY_BUFFER;
        break;
    }
    case PixelPackBuffer:
    {
        return GL_PIXEL_PACK_BUFFER;
        break;
    }
    case PixelUnpackBuffer:
    {
        return GL_PIXEL_UNPACK_BUFFER;
        break;
    }
    }

    return 0;
}

int32_t VBO::_usageToGL(UsageType usage)
{
    int32_t usageEnum = 0;
    switch (usage)
    {
    case StaticDraw:
    {
        usageEnum = GL_STATIC_DRAW;
        break;
    }
    case StaticRead:
    {
        usageEnum = GL_STATIC_READ;
        break;
    }
    case StaticCopy:
    {
        usageEnum = GL_STATIC_COPY;
        break;
    }
    case DynamicDraw:
    {
        usageEnum = GL_DYNAMIC_DRAW;
        break;
    }
    case DynamicRead:
    {
        usageEnum = GL_DYNAMIC_READ;
        break;
    }
    case DynamicCopy:
    {
        usageEnum = GL_DYNAMIC_COPY;
        break;
    }
    case StreamDraw:
    {
        usageEnum = GL_STREAM_DRAW;
        break;
    }
    case StreamRead:
    {
        usageEnum = GL_STREAM_READ;
        break;
    }
    case StreamCopy:
    {
        usageEnum = GL_STREAM_COPY;
        break;
    }
    }

    return usageEnum;
}

int32_t VBO::_mappingToGL(MappingType type)
{
    switch (type)
    {
    case ReadOnly:
    {
        return GL_READ_ONLY;
        break;
    }
    case WriteOnly:
    {
        return GL_WRITE_ONLY;
        break;
    }
    case ReadWrite:
    {
        return GL_READ_WRITE;
        break;
    }
    }

    return 0;
}