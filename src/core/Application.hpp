#pragma once

#include "../platform/Window.hpp"
#include "../graphics/GraphicsPipeline.hpp"
#include "../graphics/VulkanDevice.hpp"

namespace GameEngine
{
  namespace Core
  {
    class Application
    {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      void run();

    private:
      Platform::Window window{WIDTH, HEIGHT, "GhostEngine Window"};

      Graphics::VulkanDevice vulkanDevice{window};
      // Thes files are relative to where the build command is executed
      Graphics::GraphicsPipeline graphicsPipeline{vulkanDevice, "Shaders/simple_shader.vert.spv",
                                                  "Shaders/simple_shader.frag.spv",
                                                  Graphics::GraphicsPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };

  } // namespace Core

} // namespace GameEngine
