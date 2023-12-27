#include "myth_window.hpp"
#include <GLFW/glfw3.h>

// STD
#include <stdexcept>

namespace myth_engine {

MythWindow::MythWindow(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name} {

  initWindow();
}

MythWindow::~MythWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void MythWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API,
                 GLFW_NO_API); // Disables OpenGL context as we are using Vulkan
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  // glfwCreateWindow is expecting a c style string so windowName needs to be
  // converted from a std::string to a c_str (C style string)
  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}
void MythWindow::createWindowSurface(VkInstance instance,
                                     VkSurfaceKHR *surface) {

  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Surface failed to create window surface");
  }
}

void MythWindow::framebufferResizeCallback(GLFWwindow *window, int width,
                                           int height) {
  auto mythWindow =
      reinterpret_cast<MythWindow *>(glfwGetWindowUserPointer(window));

  mythWindow->frameBufferResized = true;
  mythWindow->width = width;
  mythWindow->height = height;
}

} // namespace myth_engine
