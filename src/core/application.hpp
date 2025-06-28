#pragma once

#include "../platform/Window.hpp"
#include "../graphics/graphics_pipeline.hpp"
#include "../graphics/vulkan_device.hpp"
#include "../graphics/swap_chain.hpp"
#include "game_object.hpp"

// std
#include <memory>
#include <vector>

namespace GameEngine
{
  namespace Core
  {
    class Application
    {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      Application();
      ~Application();

      // Copy constructors (Because the app is now managing vulkan objects we need to delete copy constructors)
      Application(const Platform::VulkanWindow&) = delete;

      void run();

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

      Platform::VulkanWindow vulkanWindow{WIDTH, HEIGHT, "GhostEngine Window"};
      Graphics::VulkanDevice vulkanDevice{vulkanWindow};

      std::unique_ptr<Graphics::SwapChain> swapChain;

      // Reason for using smart pointer is so we dont have to call new and delete for every pipeline
      // (https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/)
      std::unique_ptr<Graphics::GraphicsPipeline> pipeline;

      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::vector<GameObject> gameObjects;
    };

  } // namespace Core

} // namespace GameEngine
