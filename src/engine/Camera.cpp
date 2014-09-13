#include "Camera.h"

Camera::Camera()
{
    // the user expects y to be up
    setView(glm::vec3(0., 0., 0.f), glm::vec3(0., 0., 1.f),
            glm::vec3(0., 1., 0.f));
}

void Camera::setOrthographic(float left, float right, float bottom, float top,
                             float zNear, float zFar)
{
    mProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::setView(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    mViewMatrix = glm::lookAt(eye, center, up);
}

const glm::mat4 &Camera::getProjectionMatrix() const
{
    return mProjectionMatrix;
}

const glm::mat4 &Camera::getViewMatrix() const
{
    return mViewMatrix;
}
