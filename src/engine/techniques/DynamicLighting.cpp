#include <GL/glew.h>
#include <GL/gl.h>

#include "DynamicLighting.h"
#include "Light.h"
#include "Logging.h"
#include "Sampler.h"
#include "Shader.h"
#include "Program.h"
#include "Texture.h"
#include "Uniform.h"

#include <string>

// parameters
static const char* lightingVertexShaderText = 
"vec3 computeLighting(mat4 viewMatrix, vec4 vertex) {\n"
"  return (viewMatrix * vec4(vertex.xyz, 1.f)).xyz;\n"
"}\n";

// parameters
//	1 - number of lights that are active
static const char* lightingFragmentShaderText = 
"uniform sampler2D lightSampler;\n"
"vec4 computeLighting(vec4 color, vec3 position) {\n"
"  vec4 outColor;"
"  for(int ii=0; ii<%d; ++ii) {\n"
"    vec4 lightPosInfo   = texelFetch(lightSampler, ivec2(0,ii), 0);\n"
"    vec4 lightColorInfo = texelFetch(lightSampler, ivec2(1,ii), 0);\n"
"	 vec3 distanceVec = lightPosInfo.xyz - position;\n"
"    float distanceSqr = dot(distanceVec, distanceVec);\n"
"	 outColor += lightColorInfo.xyz * color.xyz * (distanceSqr / lightPosInfo.w);\n"
"  }\n"
"}\n";

DynamicLighting::DynamicLighting()
	: mIsDirty(false)
{
	;
}

void DynamicLighting::setProgram(std::shared_ptr<Program> program)
{
	mProgram = program;
	mIsDirty = true;
}

void DynamicLighting::addLight(LightPtr light)
{
	mLights.push_back(light);
}

std::shared_ptr<Light> DynamicLighting::getLight(int idx)
{
	return mLights.at(idx);
}

int DynamicLighting::getLightCount() const
{
	return mLights.size();
}

int DynamicLighting::getActiveLightCount() const
{
	// count the number of active lights
	std::vector<LightPtr>::const_iterator itr;
	int nbActive = 0;
	for(itr=mLights.begin(); itr!=mLights.end(); ++itr)
	{
		if((*itr)->getIsEnabled())
		{
			++nbActive;
		}
	}

	return nbActive;
}

int DynamicLighting::getActiveLightCountMax() const
{
	return sMaxActiveLights;
}

void DynamicLighting::removeLight(int idx)
{
	if(mLights.size() <= 1)
	{
		mLights.clear();
	}
	else {
		int size = (int)mLights.size();
		if(idx!=size-1)
		{
			// swap the target with the last entity 
			mLights[idx] = mLights[size-1];
		}

		// pop the last entity
		mLights.pop_back();
	}
}

void DynamicLighting::setDirty()
{
	mIsDirty = true;
}

void DynamicLighting::begin()
{
	processDirty();

	glActiveTexture(GL_TEXTURE0 + sTextureUnitUsed);
	mLightTexture->bind();
	mLightSamplerUniform->set(sTextureUnitUsed);
	mLightSampler->bind(sTextureUnitUsed);
}

void DynamicLighting::end()
{
	;
}

void DynamicLighting::processDirty()
{
	if(mIsDirty)
	{
		mIsDirty = false;
		buildObjects();
		buildShaders();
	}
}

void DynamicLighting::buildObjects()
{
	Info("Building light info texture");

	// build the texture info from the active textures
	int activeLightCount = 0;
	std::vector<float> lightData;
	for(int ii=0; ii<(int)mLights.size(); ++ii)
	{
		if(mLights[ii]->getIsEnabled())
		{
			if(activeLightCount<sMaxActiveLights)
			{
				glm::vec3 color    = mLights[ii]->getColor();
				glm::vec3 position = mLights[ii]->getPosition();
				float radius       = mLights[ii]->getRadius();

				lightData.push_back(position.x);
				lightData.push_back(position.y);
				lightData.push_back(position.z);
				lightData.push_back(radius*radius);
				
				lightData.push_back(color.x);
				lightData.push_back(color.y);
				lightData.push_back(color.z);
				
				++activeLightCount;
			}
			else 
			{
				Warning("Exceeded maximum number of active lights. This light will be ignored.");
			}
		}
	}

	// build texture data then update the texture
	TextureData lightTextureData(2, activeLightCount, 4, TextureData::Texel_F32, (unsigned char*)&lightData[0]);
	mLightTexture.reset(new Texture());
	mLightTexture->setFromData(lightTextureData);
}

void DynamicLighting::buildShaders()
{
	Info("Building shaders / uniforms");

	// set up the basic uniforms etc
	mLightSampler.reset( new Sampler() );
	mLightSamplerUniform.reset( new Uniform( Uniform::Uniform_sampler2D, "lightSampler" ) );

	mProgram->addUniform(mLightSamplerUniform);

	const int shaderTextMax = 10240;
	char shaderText[shaderTextMax];
	
	//\TODO: consider eliminating the call to getActiveLightCount if possible
	snprintf(shaderText, shaderTextMax, lightingFragmentShaderText, getActiveLightCount());
	std::shared_ptr<Shader> fragShader( new Shader(Shader::Fragment) );
	fragShader->setText(shaderText);
	fragShader->compile();

	// build the vertex shader
	snprintf(shaderText, shaderTextMax, lightingVertexShaderText);
	std::shared_ptr<Shader> vertexShader( new Shader(Shader::Vertex) );
	vertexShader->setText(shaderText);
	vertexShader->compile();

	// add the shaders to the program
	mProgram->addShader(vertexShader);
	mProgram->addShader(fragShader);
}