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
      recreateSwapChain();
      createCommandBuffers();
    }

    Application::~Application() { vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr); }

    void Application::run()
    {
      while(!Application::vulkanWindow.shouldClose())
        {
          // while window dows not close, poll events
          glfwPollEvents();
          drawFrame();
        }

      // Block CPU until GPU operations have completed
      // This way we know its save to clean up resources knowing they are no longer in use
      vkDeviceWaitIdle(vulkanDevice.device());
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
      assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
      assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

      Graphics::PipelineConfigInfo pipelineConfig{};
      Graphics::GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.renderPass = swapChain->getRenderPass();
      pipelineConfig.pipelineLayout = pipelineLayout;
      pipeline = std::make_unique<Graphics::GraphicsPipeline>(vulkanDevice, "Shaders/simple_shader.vert.spv",
                                                              "Shaders/simple_shader.frag.spv", pipelineConfig);
    };

    void Application::recreateSwapChain()
    {
      auto extent = vulkanWindow.getExtent();
      while(extent.width == 0 || extent.height == 0)
        {
          extent = vulkanWindow.getExtent();
          glfwWaitEvents();
        }

      vkDeviceWaitIdle(vulkanDevice.device());

      if(swapChain == nullptr) { swapChain = std::make_unique<Graphics::SwapChain>(vulkanDevice, extent); }
      else
        {
          swapChain = std::make_unique<Graphics::SwapChain>(vulkanDevice, extent, std::move(swapChain));
          // Free command buffers and create new ones
          if(swapChain->imageCount() != commandBuffers.size())
            {
              freeCommandBuffers();
              createCommandBuffers();
            }
        }

      createPipeline();
    }

    void Application::createCommandBuffers()
    {
      commandBuffers.resize(swapChain->imageCount());

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = vulkanDevice.getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if(vkAllocateCommandBuffers(vulkanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Application::freeCommandBuffers()
    {
      vkFreeCommandBuffers(vulkanDevice.device(), vulkanDevice.getCommandPool(),
                           static_cast<float>(commandBuffers.size()), commandBuffers.data());
      commandBuffers.clear();
    }

    void Application::recordCommandBuffer(int imageIndex)
    {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to begin recording command buffer!");
        }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = swapChain->getRenderPass();
      renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      // Setup viewport scissor with swapchain dimensions
      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
      viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
      vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
      vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

      renderGameObjects(commandBuffers[imageIndex]); // Render Game Objects

      // Finish recording
      vkCmdEndRenderPass(commandBuffers[imageIndex]);
      if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
          throw std::runtime_error("Failes to record command buffer");
        }
    }

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

    void Application::drawFrame()
    {
      uint32_t imageIndex;
      auto result = swapChain->acquireNextImage(&imageIndex);

      // If window is resized check if compatible with swapchain. If not then recreate swapchain
      if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
          recreateSwapChain();
          return;
        }

      if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
          throw std::runtime_error("failed to aquire swap chain image!");
        }

      recordCommandBuffer(imageIndex);

      // A swapchain no longer matches the surface(device) exactly but can still be used to present surface
      if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanWindow.wasVulkanWindowResized())
        {
          vulkanWindow.resetVulkanWindowResizedFlag();
          recreateSwapChain();
          return;
        };
      result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
      if(result != VK_SUCCESS) { throw std::runtime_error("failed to present swap chain image!"); }
    }

  } // namespace Core
} // namespace GameEngine
