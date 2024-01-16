//===-- myth_renderer.h ---------------------------------------------------===//
//
// Part of the Myth Engine.
//
//===----------------------------------------------------------------------===//
//
// This file declares the MythRenderer class, responsible for managing
// Vulkan rendering operations including frame handling, command buffer
// management, and swap chain operations.
//
//===----------------------------------------------------------------------===//
#ifndef MYTH_RENDERER_H
#define MYTH_RENDERER_H

#include "../graphics/myth_engine_device.hpp"
#include "../graphics/myth_engine_swap_chain.hpp"
#include "../graphics/myth_window.hpp"
#include <vulkan/vulkan_core.h>

// STD
#include <cassert>
#include <memory>
#include <vector>

namespace myth_engine
{
class MythRenderer
{

  public:
    MythRenderer(MythWindow &window, MythEngineDevice &device);
    ~MythRenderer();

    // Deleting copy and assignment constructors
    MythRenderer(const MythRenderer &) = delete;
    MythRenderer &operator=(const MythRenderer &) = delete;

    /**
     * @brief Retrieves the render pass associated with the current swap chain.
     *
     * @return VkRenderPass The render pass.
     */
    VkRenderPass getSwapChainRenderPass() const
    {
        return mythEngineSwapChain->getRenderPass();
    }
    float getAspectRatio() const
    {
        return mythEngineSwapChain->extentAspectRatio();
    }
    bool isFrameInProgress() const
    {
        return isFrameStarted;
    }

    /**
     * @brief Retrieves the current command buffer for rendering.
     *
     * @return The current command buffer.
     */
    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const
    {
        assert(isFrameStarted && "Cannot get frame index when frame is not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    MythWindow &mythWindow;
    MythEngineDevice &mythDevice;
    std::unique_ptr<MythEngineSwapChain> mythEngineSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex; // Track state of frame that is in progress
    int currentFrameIndex{0};   // Track current frame 0 to MAX_FRAMES_IN_FLIGHT
    bool isFrameStarted{false};
};

} // namespace myth_engine

#endif
