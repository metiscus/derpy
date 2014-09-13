#include "Light.h"
#include "Logging.h"

Light::Light()
    : mType(Light_Invalid)
    , mColor(0.f, 0.f, 0.f)
    , mPosition(0.f, 0.f, 0.f)
    , mRadius(0.f)
    , mIsEnabled(false)
{
    ;
}

void Light::setType(Type type)
{
    mType = type;
}

void Light::setColor(glm::vec3 color)
{
    mColor = color;
}

void Light::setPosition(glm::vec3 position)
{
    mPosition = position;
}

void Light::setRadius(float radius)
{
    mRadius = radius;
}

void Light::setIsEnabled(bool isEnabled)
{
    mIsEnabled = isEnabled;
}

Light::Type Light::getType() const
{
    return mType;
}

glm::vec3 Light::getColor() const
{
    return mColor;
}

glm::vec3 Light::getPosition() const
{
    return mPosition;
}

float Light::getRadius() const
{
    return mRadius;
}

bool Light::getIsEnabled() const
{
    return mIsEnabled;
}