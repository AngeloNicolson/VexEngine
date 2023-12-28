#ifndef MYTH_ENGINE_H
#define MYTH_ENGINE_H

#include "../graphics/myth_engine_device.hpp"
#include "../graphics/myth_engine_swap_chain.hpp"
#include "../graphics/myth_window.hpp"
#include <vulkan/vulkan_core.h>

// STD
#include <cassert>
#include <memory>
#include <vector>

namespace myth_engine {
class MythRenderer {

public:
  MythRenderer(MythWindow &window, MythEngineDevice &device);
  ~MythRenderer();

  MythRenderer(const MythRenderer &) = delete;
  MythRenderer &operator=(const MythRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return mythEngineSwapChain->getRenderPass();
  }
  bool isFrameInProgress() const { return isFrameStarted; }

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return commandBuffers[currentImageIndex];
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandbuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  MythWindow &mythWindow;
  MythEngineDevice mythDevice;
  std::unique_ptr<MythEngineSwapChain> mythEngineSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex; // Track state of frame that is in progress
  bool isFrameStarted = false;
};

} // namespace myth_engine

#endif
