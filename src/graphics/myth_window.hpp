#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace myth_engine {
class MythWindow {
public:
  MythWindow(int w, int h, std::string name);
  ~MythWindow();

  bool shouldClose() { return glfwWindowShouldClose(window); };

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(HEIGHT)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  const int WIDTH;
  const int HEIGHT;

  // Copy constructor - Disabled to prevent copying of MythWindow objects
  MythWindow(const MythWindow &) = delete;
  // Deleted copy assignment operator - Prevents assignment of MythWindow
  MythWindow &operator=(const MythWindow &) = delete;

  void initWindow();

  std::string windowName;
  GLFWwindow *window;
};
}; // namespace myth_engine

#endif // WINDOW_H
