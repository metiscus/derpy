#ifndef BATCHRENDERER_H_
#define BATCHRENDERER_H_

#include "Platform.h"
#include "Shader.h"

class Program;
class Uniform;
class Camera;

class Batch;

class BatchRenderer
{
public:
	BatchRenderer();
	~BatchRenderer();

	void begin();
	void end();
	void bindTexture(int id);

	void setShaderText( Shader::Type type, const std::string& text );
	
	std::shared_ptr<Camera> getCamera();
	void setCamera(std::shared_ptr<Camera> camera);

private:
	void _buildShaderProgram();

	bool mIsRendering;
	std::shared_ptr<Program> mProgram;
	std::shared_ptr<Uniform> mProjectionMatrixUniform;
	std::shared_ptr<Uniform> mViewMatrixUniform;
	std::shared_ptr<Uniform> mSamplerUniform;
	std::shared_ptr<Camera>  mCamera;
};

#endif /* BATCHRENDERER_H_ */
