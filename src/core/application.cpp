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

    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<Graphics::Mesh> createCubeModel(Graphics::VulkanDevice& device, glm::vec3 offset)
    {
      std::vector<Graphics::Mesh::Vertex> vertices{

        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

      };
      for(auto& v : vertices) { v.position += offset; }
      return std::make_unique<Graphics::Mesh>(device, vertices);
    }

    void Application::loadGameObjects()
    {
      std::shared_ptr<Graphics::Mesh> model = createCubeModel(vulkanDevice, {0.0f, 0.0f, 0.0f});

      auto cube = GameObject::createGameObject();
      cube.model = model;
      cube.transform.translation = {0.0f, 0.0f, 0.5f};
      cube.transform.scale = {0.5f, 0.5f, 0.5f};

      // Add cube to list of objects
      gameObjects.push_back(std::move(cube));
    };

    // Pysics and colliston systems here

  } // namespace Core
} // namespace GameEngine
