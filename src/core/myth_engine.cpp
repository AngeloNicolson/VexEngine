#include "myth_engine.hpp"
#include <vulkan/vulkan_core.h>

// STD
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
  }
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

void Engine::createCommandBuffers(){};
void Engine::drawFrame(){};

} // namespace myth_engine
