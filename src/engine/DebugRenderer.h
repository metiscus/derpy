#ifndef DEBUGRENDERER_H_
#define DEBUGRENDERER_H_

#include <glm/glm.hpp>
#include <vector>

#include "Platform.h"
#include "Noncopyable.h"

class DebugRenderer : public noncopyable
{
public:
	DebugRenderer();
	~DebugRenderer();
	
	void addLine(std::vector<glm::vec3> points, glm::vec3 color);
	
	void draw();

private:
	uint32_t mVBO;
    uint32_t mElementBuffer;
	std::vector<glm::vec3> mVerts;
	std::vector<glm::vec3> mColors;
	std::vector<int16_t>   mIndicies;
};

#endif 