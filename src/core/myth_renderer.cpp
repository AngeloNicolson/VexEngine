#include "myth_renderer.hpp"

// STD
#include <array>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace myth_engine
{

MythRenderer::MythRenderer(MythWindow &window, MythEngineDevice &device) : mythWindow{window}, mythDevice{device}
{
    recreateSwapChain();
    createCommandBuffers();
}

MythRenderer::~MythRenderer()
{
    freeCommandBuffers();
}

// Re create swap chain if window size has changed
void MythRenderer::recreateSwapChain()
{
    auto windowBounds = mythWindow.getExtent();
    while (windowBounds.width == 0 || windowBounds.height == 0)
    {
        windowBounds = mythWindow.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(mythDevice.device());
    if (mythEngineSwapChain == nullptr)
    {
        mythEngineSwapChain = std::make_unique<MythEngineSwapChain>(mythDevice, windowBounds);
    }
    else
    {
        std::shared_ptr<MythEngineSwapChain> oldSwapChain = std::move(mythEngineSwapChain);
        mythEngineSwapChain = std::make_unique<MythEngineSwapChain>(mythDevice, windowBounds, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*mythEngineSwapChain.get()))
        {
            throw std::runtime_error("Swap chain image or depth format has changed!");
            // TODO: Future improvement - Have a callback funtion notifying the app
            // that a new incompatible render pass has been created
        }
    }
    // Come back to this
}

void MythRenderer::createCommandBuffers()
{
    commandBuffers.resize(MythEngineSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    /**
     * @brief Primary command buffers CAN be submitted to a queue for execution
     * but CANNOT be called by other command buffers.
     */
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    /**
     * @brief Secondary command buffers CANNOT be submitted to a queue for
     * execution but CAN be called by other command buffers
     */
    // allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

    allocInfo.commandPool = mythDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(mythDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers!");
    };
};

void MythRenderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(mythDevice.device(), mythDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());
    commandBuffers.clear();
}

VkCommandBuffer MythRenderer::beginFrame()
{
    assert(!isFrameStarted && "Failed to present swap chain image!");

    auto result = mythEngineSwapChain->acquireNextImage(&currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    return commandBuffer;
};

void MythRenderer::endFrame()
{
    assert(isFrameStarted && "Cant call endFreame while frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }

    auto result = mythEngineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mythWindow.wasWindowResized())
    {
        mythWindow.resetWindowResizedFlag();
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % MythEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
};

void MythRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted && "Cant call beginSwapChainRenderPass if fra,e is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() &&
           "Cant begin render Pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = mythEngineSwapChain->getRenderPass();
    renderPassInfo.framebuffer = mythEngineSwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = mythEngineSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    // Record to command buffer (Begins render pass)
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(mythEngineSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(mythEngineSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, mythEngineSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
};
void MythRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted && "Cant call endSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() &&
           "Cant end render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);
};

} // namespace myth_engine
