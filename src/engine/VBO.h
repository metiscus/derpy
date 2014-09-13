#ifndef VBO_H_
#define VBO_H_

#include "Noncopyable.h"

class VBO : public noncopyable
{
public:
	enum BindMode {
		ArrayBuffer,		//!< As a vertex data buffer
		ElementArrayBuffer,	//!< As an index buffer
		PixelPackBuffer,	//!< Pack pixel buffer
		PixelUnpackBuffer,	//!< Unpack pixel buffer
	};

	enum UsageType {
		StaticDraw,		//!< Write once, draw many
		StaticRead,		//!< Write once, read many
		StaticCopy,		//!< Write once, copy many
		DynamicDraw,	//!< Write many, draw many
		DynamicRead,	//!< Write many, read many
		DynamicCopy,	//!< Write many, copy many
		StreamDraw,		//!< Write many, draw once
		StreamRead,		//!< Write many, read once
		StreamCopy		//!< Write many, copy once
	};

	VBO(BindMode mode);
	~VBO();

	void setData(void* data, size_t dataSize, UsageType usage = DynamicDraw);

	void setDataOffset(int offset, void* data, size_t dataSize);

	enum MappingType {
		ReadOnly,	//!< Only read operations
		WriteOnly,	//!< Only write operations
		ReadWrite	//!< Both read and write operations
	};

	/// Attempt to map the VBO memory into system memory for random access
	void* mapBuffer(MappingType type);

	/// Unmap the buffer
	void  unmapBuffer();

	void bind();


private:
	BindMode mMode;
	uint32_t mVBO;

	int32_t _bindToGL(BindMode mode);
	int32_t _usageToGL(UsageType type);
	int32_t _mappingToGL(MappingType type);
};

#endif // VBO_H_