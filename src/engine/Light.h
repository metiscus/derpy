#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>
#include "Noncopyable.h"

class Light : public noncopyable
{
public:
	enum Type {
		Light_Invalid,
		Light_Ambient,
		Light_Flood,
	};

	Light();

	void setType(Type type);
	void setColor(glm::vec3 color);
	void setPosition(glm::vec3 position);
	void setRadius(float radius);
	void setIsEnabled(bool isEnabled);

	Type getType() const;
	glm::vec3 getColor() const;
	glm::vec3 getPosition() const;
	float getRadius() const;
	bool getIsEnabled() const;


private:
	Type      mType;
	glm::vec3 mColor;
	glm::vec3 mPosition;
	float     mRadius;
	bool      mIsEnabled;
};

#endif // LIGHT_H_