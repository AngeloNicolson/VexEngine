#include "renderer.hpp"

// std
#include <stdexcept>
#include <array>

namespace GameEngine
{
  namespace Renderer
  {

    Renderer::Renderer(Platform::VulkanWindow& window, Graphics::VulkanDevice& device)
        : vulkanWindow{window}, vulkanDevice{device}
    {
      recreateSwapChain();
      createCommandBuffers();
    }

    // Rederer can be destroyed but Engine will continue so command buffers need freed
    Renderer::~Renderer() { freeCommandBuffers(); }

    void Renderer::recreateSwapChain()
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
          std::shared_ptr<Graphics::SwapChain> oldSwapChain = std::move(swapChain);
          swapChain = std::make_unique<Graphics::SwapChain>(vulkanDevice, extent, oldSwapChain);
          if(!oldSwapChain->compareSwapFormats(*swapChain.get()))
            {
              throw std::runtime_error("Swap chain image(or depth) format has changed!");
              /* Apparently it might be better to setup a callback function notifying the app that a new incompatable
               * renderpass has been created
               */
            }
        }
    }

    void Renderer::createCommandBuffers()
    {
      commandBuffers.resize(Graphics::SwapChain::MAX_FRAMES_IN_FLIGHT);

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

    void Renderer::freeCommandBuffers()
    {
      vkFreeCommandBuffers(vulkanDevice.device(), vulkanDevice.getCommandPool(),
                           static_cast<float>(commandBuffers.size()), commandBuffers.data());
      commandBuffers.clear();
    }

    VkCommandBuffer Renderer::beginFrame()
    {
      assert(!isFrameStarted && "Can't call beginFrame while beginFrame is already in progress");

      auto result = swapChain->acquireNextImage(&currentImageIndex);

      // If window is resized check if compatible with swapchain. If not then recreate swapchain
      if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
          recreateSwapChain();
          return nullptr; // Frame has not successfully started
        }

      if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
          throw std::runtime_error("failed to aquire swap chain image!");
        }

      isFrameStarted = true;

      // Begin command Buffer
      auto commandBuffer = getCurrentCommandBuffer();
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to begin recording command buffer!");
        }
      return commandBuffer;
    };

    void Renderer::endFrame()
    {
      assert(isFrameStarted && "Can't call endFrame when frame is not in progress");
      auto commandBuffer = getCurrentCommandBuffer();
      if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
          throw std::runtime_error("Failes to record command buffer");
        }

      auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      // A swapchain no longer matches the surface(device) exactly but can still be used to present surface
      if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanWindow.wasVulkanWindowResized())
        {
          vulkanWindow.resetVulkanWindowResizedFlag();
          recreateSwapChain();
        }
      else if(result != VK_SUCCESS) { throw std::runtime_error("failed to present swap chain image!"); }

      isFrameStarted = false;
      currentFrameIndex = (currentImageIndex + 1) % Graphics::SwapChain::MAX_FRAMES_IN_FLIGHT;
    };

    void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
      assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() &&
             "Can't begine render pass on command buffer from a different frame");

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = swapChain->getRenderPass();
      renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      // Setup viewport scissor with swapchain dimensions
      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
      viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    };
    void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
      assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() &&
             "Can't end render pass on command buffer from a different frame");

      // Finish recording
      vkCmdEndRenderPass(commandBuffer);
    };

  } // namespace Renderer
} // namespace GameEngine
