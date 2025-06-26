#pragma once

#include "../platform/Window.hpp"
#include "../graphics/GraphicsPipeline.hpp"
#include "../graphics/VulkanDevice.hpp"
#include "../graphics/swap_chain.hpp"
#include "../graphics/mesh.hpp"

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
      Application(const Platform::Window&) = delete;

      void run();

    private:
      void loadModels();
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void freeCommandBuffers();
      void drawFrame();
      void recreateSwapChain();
      void recordCommandBuffer(int imageIndex);

      Platform::Window vulkanWindow{WIDTH, HEIGHT, "GhostEngine Window"};
      Graphics::VulkanDevice vulkanDevice{vulkanWindow};

      std::unique_ptr<Graphics::SwapChain> swapChain;

      // Reason for using smart pointer is so we dont have to call new and delete for every pipeline
      // (https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/)
      std::unique_ptr<Graphics::GraphicsPipeline> pipeline;

      VkPipelineLayout pipelineLayout;
      std::vector<VkCommandBuffer> commandBuffers;
      std::unique_ptr<Graphics::Mesh> mesh;
    };

  } // namespace Core

} // namespace GameEngine
