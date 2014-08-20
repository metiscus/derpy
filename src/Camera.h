#ifndef CAMERA_H_
#define CAMERA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera 
{
public:
    Camera();
    void setOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);
    void setView(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

    const glm::mat4& getProjectionMatrix() const;
    const glm::mat4& getViewMatrix() const;

private:
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
};

#endif /* CAMERA_H_ */
