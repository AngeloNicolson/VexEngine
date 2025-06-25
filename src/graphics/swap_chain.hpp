#pragma once

#include "VulkanDevice.hpp"

// Vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <vector>

namespace GameEngine
{
  namespace Graphics
  {

    class SwapChain
    {
    public:
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      SwapChain(VulkanDevice& deviceRef, VkExtent2D windowExtent);
      ~SwapChain();

      SwapChain(const SwapChain&) = delete;
      void operator=(const SwapChain&) = delete;

      VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
      VkRenderPass getRenderPass() { return renderPass; }
      VkImageView getImageView(int index) { return swapChainImageViews[index]; }
      size_t imageCount() { return swapChainImages.size(); }
      VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
      VkExtent2D getSwapChainExtent() { return swapChainExtent; }
      uint32_t width() { return swapChainExtent.width; }
      uint32_t height() { return swapChainExtent.height; }

      float extentAspectRatio()
      {
        return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
      }
      VkFormat findDepthFormat();

      VkResult acquireNextImage(uint32_t* imageIndex);
      VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

    private:
      void createSwapChain();
      void createImageViews();
      void createDepthResources();
      void createRenderPass();
      void createFramebuffers();
      void createSyncObjects();

      // Helper functions
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
      /**
       * @breif This is the present mode for frames
       *
       * Other Modes FIFO, Mailbox, Immediate. See notes.txt for pros and cons of each mode
       **/
      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

      VkFormat swapChainImageFormat;
      VkExtent2D swapChainExtent;

      std::vector<VkFramebuffer> swapChainFramebuffers;
      VkRenderPass renderPass;

      std::vector<VkImage> depthImages;
      std::vector<VkDeviceMemory> depthImageMemories;
      std::vector<VkImageView> depthImageViews;
      std::vector<VkImage> swapChainImages;
      std::vector<VkImageView> swapChainImageViews;

      Graphics::VulkanDevice& device;
      VkExtent2D windowExtent;

      VkSwapchainKHR swapChain;

      /**
       * @breif These handle the state of the images has been aquired and os redy for rendering. Onother one to signal
       *that rendering has finnished adn presentation can happen
       *
       **/
      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      size_t currentFrame = 0;
    };
  } // namespace Graphics
} // namespace GameEngine
