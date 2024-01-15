#include "myth_camera.hpp"

// std
#include <cassert>
#include <limits>

namespace myth_engine
{

void MythCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
{
    projectionMatrix = glm::mat4{1.0f};
    projectionMatrix[0][0] = 2.0f / (right - bottom);
    projectionMatrix[1][1] = 2.0f / (bottom - top);
    projectionMatrix[2][2] = 2.0f / (far - near);
    projectionMatrix[3][0] = -(right + left) / (right - left);
    projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
    projectionMatrix[3][2] = -near / (far - near);
}

void MythCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{

    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    const float tanHalfFovy = tan(fovy / 2.0f);
    projectionMatrix = glm::mat4{1.0f};
    projectionMatrix[0][0] = 1.0f / (aspect * tanHalfFovy);
    projectionMatrix[1][1] = 1.0f / (tanHalfFovy);
    projectionMatrix[2][2] = far / (far - near);
    projectionMatrix[2][3] = 1.f;
    projectionMatrix[3][2] = -(far * near) / (far - near);
}

} // namespace myth_engine
