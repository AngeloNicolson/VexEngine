#pragma once

#include "../platform/Window.hpp"
#include "../graphics/vulkan_device.hpp"
#include "../renderer/renderer.hpp"
#include "game_object.hpp"

// std
#include <vector>

namespace GameEngine
{
  namespace Core
  {
    class Application
    {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      Application();
      ~Application();

      // Copy constructors (Because the app is now managing vulkan objects we need to delete copy constructors)
      Application(const Platform::VulkanWindow&) = delete;

      void run();

    private:
      void loadGameObjects();

      Platform::VulkanWindow vulkanWindow{WIDTH, HEIGHT, "GhostEngine Window"};
      Graphics::VulkanDevice vulkanDevice{vulkanWindow};
      Renderer::Renderer renderer{vulkanWindow, vulkanDevice};

      std::vector<GameObject> gameObjects;
    };

  } // namespace Core

} // namespace GameEngine
