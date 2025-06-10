#pragma once

#include "../platform/Window.hpp"

namespace GameEngine
{
  namespace Core
  {
    class Application
    {
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      void run() {};

    private:
      Platform::Window window{WIDTH, HEIGHT, "Vulkan"};
    };

  } // namespace Core

} // namespace GameEngine
