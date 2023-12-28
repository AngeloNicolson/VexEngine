#ifndef MYTH_ENGINE_H
#define MYTH_ENGINE_H

#include "../graphics/myth_engine_device.hpp"
#include "../graphics/myth_engine_swap_chain.hpp"
#include "../graphics/myth_pipeline.hpp"
#include "../graphics/myth_window.hpp"
#include "myth_game_object.hpp"
#include <vulkan/vulkan_core.h>

// STD
#include <memory>
#include <vector>

namespace myth_engine {
class Engine {

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  Engine();
  ~Engine();
  void run();

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

private:
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void renderGameObjects(VkCommandBuffer commandBuffer);

  MythWindow mythWindow{WIDTH, HEIGHT, "Hello Vulkan"};
  MythEngineDevice mythDevice{mythWindow};
  std::unique_ptr<MythEngineSwapChain> mythEngineSwapChain;
  std::unique_ptr<MythPipeline> mythPipeline;
  VkPipelineLayout pipelineLayout;

  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<MythGameObject> gameObjects;
};

} // namespace myth_engine

#endif
