#pragma once

#include "../graphics/graphics_pipeline.hpp"
#include "../graphics/vulkan_device.hpp"
#include "../core/game_object.hpp"

// std
#include <memory>
#include <vector>

namespace GameEngine
{
  namespace Core
  {
    class RenderSystem
    {
    public:
      RenderSystem(Graphics::VulkanDevice& device, VkRenderPass renderPass);
      ~RenderSystem();

      // Copy constructors (Because the app is now managing vulkan objects we need to delete copy constructors)
      RenderSystem(const RenderSystem&) = delete;
      RenderSystem& operator=(const RenderSystem&) = delete;

      void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<Core::GameObject>& gameObjects);

    private:
      void createPipelineLayout();
      void createPipeline(VkRenderPass renderPass);

      Graphics::VulkanDevice& vulkanDevice;

      // Reason for using smart pointer is so we dont have to call new and delete for every pipeline
      // (https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/)
      std::unique_ptr<Graphics::GraphicsPipeline> pipeline;

      VkPipelineLayout pipelineLayout;
    };

  } // namespace Core

} // namespace GameEngine
