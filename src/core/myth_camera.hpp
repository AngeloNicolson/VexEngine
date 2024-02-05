//===-- myth_camera.hpp ---------------------------------------------------===//
//
// Part of the Myth Engine core.
//
//===----------------------------------------------------------------------===//
//
// This file declares the MythCamera class, responsible for managing
// camera-related functionalities, including setting different types of
// projections and retrieving the projection matrix.
//
//===----------------------------------------------------------------------===//
#ifndef MYTH_CAMERA_HPP
#define MYTH_CAMERA_HPP

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace myth_engine
{

class MythCamera
{

  public:
    void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.0f, -1.f, 0.0f});
    void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.0f, -1.f, 0.0f});
    void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

    // Gets the projection matrix
    const glm::mat4 &getProjection() const
    {
        return projectionMatrix;
    }

    const glm::mat4 &getview() const
    {
        return viewMatrix;
    }

  private:
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 viewMatrix{1.f};
};

} // namespace myth_engine

#endif
