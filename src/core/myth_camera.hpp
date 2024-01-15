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

    // Gets the projection matrix
    const glm::mat4 &getProjection() const
    {
        return projectionMatrix;
    }

  private:
    glm::mat4 projectionMatrix{1.0f};
};

} // namespace myth_engine

#endif
