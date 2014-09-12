#ifndef DYNAMIC_LIGHTING_H_
#define DYNAMIC_LIGHTING_H_

#include "Noncopyable.h"
#include "Platform.h"

#include <vector>

class Light;
class Sampler;
class Texture;
class Uniform;
class Program;

class DynamicLighting : public noncopyable
{
public:
	typedef std::shared_ptr<Light> LightPtr;
	
	DynamicLighting();

	void setProgram(std::shared_ptr<Program> program);

	void addLight(LightPtr light);
	std::shared_ptr<Light> getLight(int idx);
	int getLightCount() const;
	int getActiveLightCount() const;
	int getActiveLightCountMax() const;
	void removeLight(int idx);
	void setDirty();

	void begin();
	void end();

private:
	bool mIsDirty;
	std::shared_ptr<Sampler> mLightSampler;
	std::shared_ptr<Uniform> mLightSamplerUniform;
	std::shared_ptr<Texture> mLightTexture;
	std::shared_ptr<Program> mProgram;
	std::vector<LightPtr>    mLights;

	// this is the maximum number of lights the technique supports being active at once
	static const int sMaxActiveLights = 20;

	// this is the GL_TEXTURE0 + ?? unit id used for the textures
	static const int sTextureUnitUsed = 2;

	void processDirty();
	void buildObjects();
	void buildShaders();
};

#endif