#ifndef DEBUGRENDERER_H_
#define DEBUGRENDERER_H_

#include <glm/glm.hpp>
#include <vector>

#include "Platform.h"
#include "Noncopyable.h"

class Camera;
class Program;
class Uniform;
class VBO;

class DebugRenderer : public noncopyable
{
public:
	DebugRenderer();
	~DebugRenderer();

	void addLines(std::vector<glm::vec3> points, glm::vec3 color);
	
	void draw(std::shared_ptr<Camera> camera);

	void clear();

private:
	std::shared_ptr<VBO> mVBO;
    std::shared_ptr<VBO> mElementBuffer;
    std::shared_ptr<Program> mProgram;
    std::shared_ptr<Uniform> mProjectionMatrixUniform;
    std::shared_ptr<Uniform> mViewMatrixUniform;
	std::vector<glm::vec3> mVerts;
	std::vector<glm::vec3> mColors;
	std::vector<uint16_t>  mIndices;

	bool mIsDirty;
	int32_t mPrimitiveCount;
};

#endif 