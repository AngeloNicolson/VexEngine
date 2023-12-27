#include "myth_engine.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <vulkan/vulkan_core.h>

// STD
#include <array>
#include <iostream>
#include <stdexcept>

namespace myth_engine {

Engine::Engine() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
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

void Engine::loadModels() {
  std::vector<MythVertexBufferManager::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  // for (const auto &vertex : vertices) {
  //   std::cout << "Vertex Position: (" << vertex.position.x << ", "
  //             << vertex.position.y << ")" << std::endl;
  // }
  mythVertexBuffer =
      std::make_unique<MythVertexBufferManager>(mythDevice, vertices);
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
  assert(mythEngineSwapChain != nullptr &&
         "Cannot create pipeline before swap chain");
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  MythPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = mythEngineSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;

  mythPipeline = std::make_unique<MythPipeline>(
      mythDevice, "graphics/shaders/simple_shader.vert.spv",
      "graphics/shaders/simple_shader.frag.spv", pipelineConfig);
}

// Re create swap chain if window size has changed
void Engine::recreateSwapChain() {
  auto windowBounds = mythWindow.getExtent();
  while (windowBounds.width == 0 || windowBounds.height == 0) {
    windowBounds = mythWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(mythDevice.device());
  if (mythEngineSwapChain == nullptr) {
    mythEngineSwapChain =
        std::make_unique<MythEngineSwapChain>(mythDevice, windowBounds);

  } else {
    mythEngineSwapChain = std::make_unique<MythEngineSwapChain>(
        mythDevice, windowBounds, std::move(mythEngineSwapChain));
    freeCommandBuffers();
    createCommandBuffers();
  }

  // If renderPass compatible do nothing else.
  createPipeline();
}

void Engine::createCommandBuffers() {
  commandBuffers.resize(mythEngineSwapChain->imageCount());
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
};

void Engine::freeCommandBuffers() {
  vkFreeCommandBuffers(mythDevice.device(), mythDevice.getCommandPool(),
                       static_cast<uint32_t>(commandBuffers.size()),
                       commandBuffers.data());
  commandBuffers.clear();
}

void Engine::recordCommandBuffer(int imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = mythEngineSwapChain->getRenderPass();
  renderPassInfo.framebuffer = mythEngineSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = mythEngineSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  // Record to command buffer (Begins render pass)
  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width =
      static_cast<float>(mythEngineSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(mythEngineSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, mythEngineSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  // Binding the Graphics pipeline
  mythPipeline->bind(commandBuffers[imageIndex]);

  // Binding the Vertex Buffer
  mythVertexBuffer->bind(commandBuffers[imageIndex]);

  // Issue a draw command using the bound vertex buffer
  mythVertexBuffer->draw(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer!");
  }
}

/**
 * @brief Executes the necessary steps to draw a frame onto the surface.
 *
 * This function handles the process of acquiring an image from the swap chain,
 * submitting command buffers for rendering, and presenting the frame.
 * If the swap chain needs recreation due to window resizing or other issues, it
 * triggers the recreation.
 *
 **/
void Engine::drawFrame() {
  uint32_t imageIndex;
  auto result = mythEngineSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result = mythEngineSwapChain->submitCommandBuffers(
      &commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      mythWindow.wasWindowResized()) {
    mythWindow.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

} // namespace myth_engine
