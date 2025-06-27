// This will handle window creation

#include "Window.hpp"

// std
#include <stdexcept>

namespace GameEngine
{
  namespace Platform
  {
    VulkanWindow::VulkanWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} { initWindow(); }

    VulkanWindow::~VulkanWindow()
    {
      glfwDestroyWindow(window);
      glfwTerminate();
    }

    void VulkanWindow::initWindow()
    {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(window, this);
      glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
      if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to craete window surface");
        }
    }

    void VulkanWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
      auto lveWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
      lveWindow->frameBufferResized = true;
      lveWindow->width = width;
      lveWindow->height = height;
    }

  } // namespace Platform
} // namespace GameEngine
