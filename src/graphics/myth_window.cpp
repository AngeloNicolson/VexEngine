#include "myth_window.hpp"
#include <GLFW/glfw3.h>

// STD
#include <stdexcept>

namespace myth_engine {

MythWindow::MythWindow(int w, int h, std::string name)
    : WIDTH{w}, HEIGHT{h}, windowName{name} {

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
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Dissable window resize

  // glfwCreateWindow is expecting a c style string so windowName needs to be
  // converted from a std::string to a c_str (C style string)
  window =
      glfwCreateWindow(WIDTH, HEIGHT, windowName.c_str(), nullptr, nullptr);
}
void MythWindow::createWindowSurface(VkInstance instance,
                                     VkSurfaceKHR *surface) {

  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Surface failed to create window surface");
  }
}
} // namespace myth_engine
