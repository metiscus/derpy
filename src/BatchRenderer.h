#ifndef BATCHRENDERER_H_
#define BATCHRENDERER_H_

#include "Platform.h"

class Program;
class Uniform;
class Camera;

class BatchRenderer
{
public:
	BatchRenderer();
	~BatchRenderer();
	
	void draw(const Camera& camera);
	void bindTexture(int id);

private:
	void _buildShaderProgram();
	
	std::shared_ptr<Program> mProgram;
	std::shared_ptr<Uniform> mProjectionMatrixUniform;
	std::shared_ptr<Uniform> mViewMatrixUniform;
	std::shared_ptr<Uniform> mSamplerUniform;
};

#endif /* BATCHRENDERER_H_ */
