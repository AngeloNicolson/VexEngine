#include "application.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <array>

namespace GameEngine
{
  namespace Core
  {
    // This needs to align with Vulkan specification which requires 16 byte padding
    struct SimplePushConstantData
    {
      glm::mat2 transform{1.0f}; // default initialization to identity matrix
      glm::vec2 offset;
      float padding2[2]; // Offset the color to compensate for vec2
      glm::vec3 color;
    };

    Application::Application()
    {
      loadGameObjects();
      createPipelineLayout();
      createPipeline();
    }

    Application::~Application() { vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr); }

    void Application::run()
    {
      while(!Application::vulkanWindow.shouldClose())
        {
          // while window dows not close, poll events
          glfwPollEvents();

          // Begin fram function will return a nullptr if swapchain needs to be created
          if(auto commandBuffer = renderer.beginFrame())
            {
              renderer.beginSwapChainRenderPass(commandBuffer);
              renderGameObjects(commandBuffer);
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

    // Pipeline Layout
    void Application::createPipelineLayout()
    {
      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      pushConstantRange.size = sizeof(SimplePushConstantData);

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{}; // struct

      // Struct member variables
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pSetLayouts = nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 1;
      pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

      if(vkCreatePipelineLayout(vulkanDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to create pipeline layout!");
        };
    }

    // Pipeline
    void Application::createPipeline()
    {
      assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

      Graphics::PipelineConfigInfo pipelineConfig{};
      Graphics::GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
      pipelineConfig.pipelineLayout = pipelineLayout;
      pipeline = std::make_unique<Graphics::GraphicsPipeline>(vulkanDevice, "Shaders/simple_shader.vert.spv",
                                                              "Shaders/simple_shader.frag.spv", pipelineConfig);
    };

    void Application::renderGameObjects(VkCommandBuffer commandBuffer)
    {
      pipeline->bind(commandBuffer);

      // Loop over game objects
      for(auto& obj : gameObjects)
        {
          SimplePushConstantData push{};
          // Order must match the uniform push constant in the shader.vert
          push.offset = obj.transform2d.translation;
          push.color = obj.color;
          push.transform = obj.transform2d.mat2();

          vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                             0, sizeof(SimplePushConstantData), &push);

          obj.model->bind(commandBuffer);
          obj.model->draw(commandBuffer);
        }
    };

  } // namespace Core
} // namespace GameEngine
