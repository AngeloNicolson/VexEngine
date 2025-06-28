#include "application.hpp"
#include "../renderer/render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace GameEngine
{
  namespace Core
  {

    Application::Application() { loadGameObjects(); }
    Application::~Application() {}

    void Application::run()
    {
      // Initalize renderSystem
      RenderSystem renderSystem{vulkanDevice, renderer.getSwapChainRenderPass()};

      while(!Application::vulkanWindow.shouldClose())
        {
          // while window dows not close, poll events
          glfwPollEvents();

          // Begin fram function will return a nullptr if swapchain needs to be created
          if(auto commandBuffer = renderer.beginFrame())
            {
              renderer.beginSwapChainRenderPass(commandBuffer);
              renderSystem.renderGameObjects(commandBuffer, gameObjects);
              renderer.endSwapChainRenderPass(commandBuffer);
              renderer.endFrame();
            }

          // Block CPU until GPU operations have completed
          // This way we know its save to clean up resources knowing they are no longer in use
          vkDeviceWaitIdle(vulkanDevice.device());
        }
    }

    void Application::loadGameObjects()
    {
      std::vector<Graphics::Mesh::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

      // Used shared allows us to use one model instace between many game objects
      auto mesh = std::make_shared<Graphics::Mesh>(vulkanDevice, vertices);

      auto triangle = GameObject::createGameObject();
      triangle.model = mesh;
      triangle.color = {0.1f, 0.8f, 0.1f};
      triangle.transform2d.translation.x = 0.2f;
      triangle.transform2d.scale = {0.2f, 0.2f}; // Scale the objects 2x2 matrix x and y components
      triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

      gameObjects.push_back(std::move(triangle));
    };

    // Pysics and colliston systems here

  } // namespace Core
} // namespace GameEngine
