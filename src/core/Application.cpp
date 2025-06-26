#include "Application.hpp"

#include <stdexcept>
#include <array>

namespace GameEngine
{
  namespace Core
  {
    Application::Application()
    {
      loadModels();
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

    void Application::loadModels()
    {
      std::vector<Graphics::Mesh::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

      mesh = std::make_unique<Graphics::Mesh>(vulkanDevice, vertices);
    };

    // Pipeline Layout
    void Application::createPipelineLayout()
    {
      VkPipelineLayoutCreateInfo pipelineLayoutInfo{}; // struct

      // Struct member variables
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pSetLayouts = nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 0;
      pipelineLayoutInfo.pPushConstantRanges = nullptr;

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
      clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
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

      pipeline->bind(commandBuffers[imageIndex]);
      mesh->bind(commandBuffers[imageIndex]);
      mesh->draw(commandBuffers[imageIndex]);

      // Finish recording
      vkCmdEndRenderPass(commandBuffers[imageIndex]);
      if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
          throw std::runtime_error("Failes to record command buffer");
        }
    }

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
      if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanWindow.wasWindowResized())
        {
          vulkanWindow.resetWindowResizedFlag();
          recreateSwapChain();
          return;
        };
      result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
      if(result != VK_SUCCESS) { throw std::runtime_error("failed to present swap chain image!"); }
    }

  } // namespace Core
} // namespace GameEngine
