#include "Application.hpp"

namespace GameEngine
{
  namespace Core
  {
    void Application::run()
    {
      while(!Application::window.shouldClose())
        {
          // while window dows not close, poll events
          glfwPollEvents();
        }
    }
  } // namespace Core
} // namespace GameEngine
