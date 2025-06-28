#pragma once

#include "../graphics/mesh.hpp"

// std
#include <memory>

namespace GameEngine
{
  namespace Core
  {

    // Transformation for 2D objects
    struct Transform2dComponent
    {
      glm::vec2 translation{}; // Position offset
      glm::vec2 scale{1.0f, 1.0f};
      float rotation;

      glm::mat2 mat2()
      {
        const float sin = glm::sin(rotation);
        const float cos = glm::sin(rotation);
        // Rotation Matrix
        glm::mat2 rotMatrix{{cos, sin}, {-sin, cos}};

        glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
        return rotMatrix * scaleMat;
      }
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

      GameObject(const GameObject&) = delete;
      GameObject& operator=(const GameObject&) = delete;
      GameObject(GameObject&&) = default;
      GameObject& operator=(GameObject&&) = default;

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
