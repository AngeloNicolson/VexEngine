#pragma once

#include "../platform/Window.hpp"
#include "../graphics/vulkan_device.hpp"
#include "../graphics/swap_chain.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace GameEngine
{
  namespace Renderer
  {
    class Renderer
    {
    public:
      Renderer(Platform::VulkanWindow& window, Graphics::VulkanDevice& device);
      ~Renderer();

      // Copy constructors (Because the app is now managing vulkan objects we need to delete copy constructors)
      Renderer(const Platform::VulkanWindow&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); };
      bool isFrameInProgress() const { return isFrameStarted; };

      VkCommandBuffer getCurrentCommandBuffer() const
      {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentImageIndex];
      }

      VkCommandBuffer beginFrame();
      void endFrame();
      void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
      void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
      void createCommandBuffers();
      void freeCommandBuffers();
      void recreateSwapChain();

      Platform::VulkanWindow& vulkanWindow;
      Graphics::VulkanDevice& vulkanDevice;

      std::unique_ptr<Graphics::SwapChain> swapChain;
      std::vector<VkCommandBuffer> commandBuffers;

      uint32_t currentImageIndex;
      bool isFrameStarted = false;
    };

  } // namespace Renderer

} // namespace GameEngine
