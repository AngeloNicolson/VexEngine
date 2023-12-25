#ifndef MYTH_ENGINE_H
#define MYTH_ENGINE_H

#include "../graphics/myth_engine_device.hpp"
#include "../graphics/myth_engine_swap_chain.hpp"
#include "../graphics/myth_pipeline.hpp"
#include "../graphics/myth_vertex_buffer_manager.hpp"
#include "../graphics/myth_window.hpp"
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
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  MythWindow mythWindow{WIDTH, HEIGHT, "Hello Vulkan"};
  MythEngineDevice mythDevice{mythWindow};
  MythEngineSwapChain mythEngineSwapChain{mythDevice, mythWindow.getExtent()};
  std::unique_ptr<MythPipeline> mythPipeline;
  VkPipelineLayout pipelineLayout;

  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<MythVertexBufferManager> mythVertexbuffer;
};

} // namespace myth_engine

#endif
