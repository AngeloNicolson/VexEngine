//===-- include/core/myth_game_engine_0bject.hpp --------------------------===//
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

// std
#include <glm/fwd.hpp>
#include <memory>

namespace myth_engine {

struct Transform2dComponent {
  glm::vec2 translation{}; // Position offset
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
    return rotMatrix * scaleMat;
  };
};

class MythGameObject {
public:
  using id_t = unsigned int;

  static MythGameObject createGameObject() {
    static id_t currentId = 0;
    return MythGameObject{currentId++};
  }

  // Cannot copy or assign
  MythGameObject(const MythGameObject &) = delete;
  MythGameObject &operator=(const MythGameObject &) = delete;
  // Default copy and assignment
  MythGameObject(MythGameObject &&) = default;
  MythGameObject &operator=(MythGameObject &&) = default;

  const id_t getId() { return id; }

  std::shared_ptr<MythVertexBufferManager> model{};
  glm::vec3 color{};

  Transform2dComponent transform2d{};

private:
  MythGameObject(id_t objId) : id{objId} {}
  id_t id;
};

} // namespace myth_engine

#endif // ENGINE_GAME_ENGINE_OBJECT_HPP
