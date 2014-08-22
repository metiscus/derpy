#ifndef MESHRENDERER_H_
#define MESHRENDERER_H_

#include "Noncopyable.h"
#include "Platform.h"
#include "Shader.h"

#include <vector>

class Program;
class Uniform;
class Camera;

class MeshRenderer : public noncopyable
{
public:
	MeshRenderer();
	~MeshRenderer();

	void begin();
	
	void end();
	
	
	void bindTexture(int id);

	void setShaderText( Shader::Type type, const std::string& text );
	
	std::shared_ptr<Camera> getCamera();
	void setCamera(std::shared_ptr<Camera> camera);
	
	void addCustomUniform( std::shared_ptr<Uniform> uniform );

private:
	void _buildShaderProgram();

	bool mIsDirty;
	bool mIsRendering;
	std::shared_ptr<Program> mProgram;
	std::shared_ptr<Uniform> mProjectionMatrixUniform;
	std::shared_ptr<Uniform> mViewMatrixUniform;
	std::shared_ptr<Uniform> mSamplerUniform;
	std::shared_ptr<Camera>  mCamera;
	std::vector<std::shared_ptr<Uniform> > mCustomUniforms;
	std::string mVertexShaderSource;
	std::string mFragmentShaderSource;
};

#endif /* MESHRENDERER_H_ */
