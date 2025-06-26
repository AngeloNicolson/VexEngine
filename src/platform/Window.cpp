// This will handle window creation

#include "Window.hpp"

// std
#include <stdexcept>

namespace GameEngine
{
  Platform::Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} { initWindow(); };

  Platform::Window::~Window()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void Platform::Window::initWindow()
  {
    glfwInit();

    // Disables this a as we are using Vulkan, not OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Disable window resizing as Vulkan needs to handle it in a different way;
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr /* Controller for fullscreen*/, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
  }

  void Platform::Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
  {
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
      {
        throw std::runtime_error("Failed to Create window surface");
      };
  }

  void Platform::Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
  {
    auto vulkanWindow = reinterpret_cast<Platform::Window*>(glfwGetWindowUserPointer(window)); // Cursor when on screen
    vulkanWindow->frameBufferResized = true;
    vulkanWindow->width = width;
    vulkanWindow->height = height;
  };

} // namespace GameEngine
