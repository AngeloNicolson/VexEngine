#include "myth_engine.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

// STD
#include <array>
#include <stdexcept>

namespace myth_engine {

Engine::Engine() {
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}

Engine::~Engine() {
  vkDestroyPipelineLayout(mythDevice.device(), pipelineLayout, nullptr);
}

void Engine::run() {
  // While window is open poll events like clicks etc.
  while (!mythWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }

  // Waits untill all gpu operations have completed.
  vkDeviceWaitIdle(mythDevice.device());
}

void Engine::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(mythDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void Engine::createPipeline() {
  auto pipelineConfig = MythPipeline::defaultPipelineConfigInfo(
      mythEngineSwapChain.width(), mythEngineSwapChain.height());
  pipelineConfig.renderPass = mythEngineSwapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;

  mythPipeline = std::make_unique<MythPipeline>(
      mythDevice, "graphics/shaders/simple_shader.vert.spv",
      "graphics/shaders/simple_shader.frag.spv", pipelineConfig);
}

void Engine::createCommandBuffers() {

  commandBuffers.resize(mythEngineSwapChain.imageCount());

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

  if (vkAllocateCommandBuffers(mythDevice.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers!");
  };

  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = mythEngineSwapChain.getRenderPass();
    renderPassInfo.framebuffer = mythEngineSwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = mythEngineSwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    // Record to command buffer (Begins render pass)
    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    mythPipeline->bind(commandBuffers[i]);
    vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to record command buffer!");
    }
  }
};
void Engine::drawFrame() {
  uint32_t imageIndex;
  auto result = mythEngineSwapChain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chai image!");
  }

  result = mythEngineSwapChain.submitCommandBuffers(&commandBuffers[imageIndex],
                                                    &imageIndex);

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

} // namespace myth_engine
