#pragma once

#include "../graphics/mesh.hpp"

// std
#include <memory>

namespace GameEngine
{
  namespace Core
  {

    struct Transform2dComponent
    {
      glm::vec2 transition{}; // Position offset

      glm::mat2 mat2() { return glm::mat2{1.0f}; }
    };

    class GameObject
    {
    public:
      using id_t = unsigned int; // This will be a unique ID

      /**
       * @breif This will create a new game object with unique ID's by incrementing the ID
       *
       **/
      static GameObject createGameObject()
      {
        static id_t currentId = 0;
        return GameObject{currentId++};
      }

      GameObject& operator=(GameObject&&) = default;
      GameObject(const GameObject&) = delete;
      GameObject& operator=(const GameObject&) = delete;
      GameObject(GameObject&&) = delete;

      const id_t getId() { return id; }

      std::shared_ptr<Graphics::Mesh> model{};
      glm::vec3 color{};
      Transform2dComponent transform2d{};

    private:
      GameObject(id_t objId) : id{objId} {};

      id_t id;
    };
  } // namespace Core

} // namespace GameEngine
