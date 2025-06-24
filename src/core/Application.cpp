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
      createPipeline();
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

    void
    sierpinski(std::vector<Graphics::Mesh::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top)
    {
      if(depth <= 0)
        {
          vertices.push_back({top});
          vertices.push_back({right});
          vertices.push_back({left});
        }
      else
        {
          auto leftTop = 0.5f * (left + top);
          auto rightTop = 0.5f * (right + top);
          auto leftRight = 0.5f * (left + right);
          sierpinski(vertices, depth - 1, left, leftRight, leftTop);
          sierpinski(vertices, depth - 1, leftRight, right, rightTop);
          sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }

    void Application::loadModels()
    {
      std::vector<Graphics::Mesh::Vertex> vertices{};
      sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
      mesh = std::make_unique<Graphics::Mesh>(vulkanDevice, vertices);
    }

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
      auto pipelineConfig =
        Graphics::GraphicsPipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
      pipelineConfig.renderPass = swapChain.getRenderPass();
      pipelineConfig.pipelineLayout = pipelineLayout;
      pipeline = std::make_unique<Graphics::GraphicsPipeline>(vulkanDevice, "Shaders/simple_shader.vert.spv",
                                                              "Shaders/simple_shader.frag.spv", pipelineConfig);
    };

    void Application::createCommandBuffers()
    {
      commandBuffers.resize(swapChain.imageCount());

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = vulkanDevice.getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if(vkAllocateCommandBuffers(vulkanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to allocate command buffers!");
        }

      for(int i = 0; i < commandBuffers.size(); i++)
        {
          VkCommandBufferBeginInfo beginInfo{};
          beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

          if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
              throw std::runtime_error("failed to begin recording command buffer!");
            }

          VkRenderPassBeginInfo renderPassInfo{};
          renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
          renderPassInfo.renderPass = swapChain.getRenderPass();
          renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

          renderPassInfo.renderArea.offset = {0, 0};
          renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

          std::array<VkClearValue, 2> clearValues{};
          clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
          clearValues[1].depthStencil = {1.0f, 0};
          renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
          renderPassInfo.pClearValues = clearValues.data();

          vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

          pipeline->bind(commandBuffers[i]);
          mesh->bind(commandBuffers[i]);
          mesh->draw(commandBuffers[i]);

          // Finish recording
          vkCmdEndRenderPass(commandBuffers[i]);
          if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
              throw std::runtime_error("Failes to record command buffer");
            }
        }
    };
    void Application::drawFrame()
    {
      uint32_t imageIndex;
      auto result = swapChain.acquireNextImage(&imageIndex);
      if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
          throw std::runtime_error("failed to aquire swap chain image!");
        }

      result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
      if(result != VK_SUCCESS) { throw std::runtime_error("failed to present swap chain image!"); }
    }

  } // namespace Core
} // namespace GameEngine
