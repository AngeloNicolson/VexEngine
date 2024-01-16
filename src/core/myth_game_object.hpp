//===-- include/core/myth_game_engine_1bject.hpp --------------------------===//
//
// Part of the Myth Engine project.
//
//===----------------------------------------------------------------------===//
//
// This file defines the GameEngineObject class, which serves as the base class
// for all game objects within the Myth game engine.
//
// Note: This will be replaced by an Entity component system
//===----------------------------------------------------------------------===//

#ifndef ENGINE_GAME_ENGINE_OBJECT_HPP
#define ENGINE_GAME_ENGINE_OBJECT_HPP

#include "../graphics/myth_vertex_buffer_manager.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace myth_engine
{

struct TransformComponent
{
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    // Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
    // Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix

    // WHen I get time, research intrinsic vs extrinsic rotations
    glm::mat4 mat4()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{{
                             scale.x * (c1 * c3 + s1 * s2 * s3),
                             scale.x * (c2 * s3),
                             scale.x * (c1 * s2 * s3 - c3 * s1),
                             0.0f,
                         },
                         {
                             scale.y * (c3 * s1 * s2 - c1 * s3),
                             scale.y * (c2 * c3),
                             scale.y * (c1 * c3 * s2 + s1 * s3),
                             0.0f,
                         },
                         {
                             scale.z * (c2 * s1),
                             scale.z * (-s2),
                             scale.z * (c1 * c2),
                             0.0f,
                         },
                         {translation.x, translation.y, translation.z, 1.0f}};
    }
};

class MythGameObject
{
  public:
    using id_t = unsigned int;

    static MythGameObject createGameObject()
    {
        static id_t currentId = 0;
        return MythGameObject{currentId++};
    }
    struct RigidBody2dComponent
    {
        glm::vec2 velocity;
        float mass{1.0f};
    };

    // Cannot copy or assign
    MythGameObject(const MythGameObject &) = delete;
    MythGameObject &operator=(const MythGameObject &) = delete;
    // Default copy and assignment
    MythGameObject(MythGameObject &&) = default;
    MythGameObject &operator=(MythGameObject &&) = default;

    const id_t getId()
    {
        return id;
    }

    std::shared_ptr<MythVertexBufferManager> model{};
    glm::vec3 color{};

    TransformComponent transform{};

  private:
    MythGameObject(id_t objId) : id{objId}
    {
    }
    id_t id;
};
} // namespace myth_engine
#endif // ENGINE_GAME_ENGINE_OBJECT_HPP
