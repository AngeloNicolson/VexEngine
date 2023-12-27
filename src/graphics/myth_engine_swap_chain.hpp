//===-- myth_engine_swap_chain.hpp ----------------------------------------===//
//
// Part of the Myth Engine, a Vulkan-based graphics engine.
//
// This file defines the MythEngineSwapChain class responsible for managing the
// Vulkan swap chain, including framebuffers, image views, render passes, depth
// resources, and synchronization objects.
//
//===----------------------------------------------------------------------===//

#ifndef MYTH_ENGINE_SWAP_CHAIN_H
#define MYTH_ENGINE_SWAP_CHAIN_H

#include "myth_engine_device.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>

namespace myth_engine {

class MythEngineSwapChain {
public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  MythEngineSwapChain(MythEngineDevice &deviceRef, VkExtent2D windowExtent);
  ~MythEngineSwapChain();

  MythEngineSwapChain(const MythEngineSwapChain &) = delete;
  MythEngineSwapChain &operator=(const MythEngineSwapChain &) = delete;

  VkFramebuffer getFrameBuffer(int index) {
    return swapChainFramebuffers[index];
  }
  VkRenderPass getRenderPass() { return renderPass; }
  VkImageView getImageView(int index) { return swapChainImageViews[index]; }
  size_t imageCount() { return swapChainImages.size(); }
  VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
  VkExtent2D getSwapChainExtent() { return swapChainExtent; }
  uint32_t width() { return swapChainExtent.width; }
  uint32_t height() { return swapChainExtent.height; }

  float extentAspectRatio() {
    return static_cast<float>(swapChainExtent.width) /
           static_cast<float>(swapChainExtent.height);
  }
  VkFormat findDepthFormat();

  VkResult acquireNextImage(uint32_t *imageIndex);
  VkResult submitCommandBuffers(const VkCommandBuffer *buffers,
                                uint32_t *imageIndex);

private:
  void createSwapChain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  // Helper functions
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkRenderPass renderPass;

  std::vector<VkImage> depthImages;
  std::vector<VkDeviceMemory> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;

  MythEngineDevice &device;
  VkExtent2D windowExtent;

  VkSwapchainKHR swapChain;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
  size_t currentFrame = 0;
};

} // namespace myth_engine
#endif
