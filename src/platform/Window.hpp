// VulkanWindow declaration
#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace GameEngine
{
  namespace Platform
  {
    /**
     * @brief Represents a native application window.
     *        Handles creation and lifetime of a GLFW window for use with Vulkan.
     */
    class VulkanWindow
    {
    public:
      /**
       * @brief Constructs the window.
       * @param w The width of the window in pixels.
       * @param h The height of the window in pixels.
       * @param name The title of the window.
       */
      VulkanWindow(int w, int h, std::string name);

      ~VulkanWindow();

      /**
       * @brief Deleted copy constructor to prevent copying of VulkanWindow instances.
       *
       * Copying a VulkanWindow object is disallowed to avoid issues with
       * resource management and ownership.
       */
      VulkanWindow(const VulkanWindow&) = delete;

      /**
       * @brief Deleted copy assignment operator to prevent copying of VulkanWindow instances.
       *
       * Assigning one VulkanWindow object to another is disallowed to maintain
       * unique ownership of underlying resources.
       *
       * @return Reference to the current VulkanWindow object.
       */
      VulkanWindow& operator=(const VulkanWindow&) = delete;

      /**
       * @brief Checks if the window should close.
       * @return true if the user has requested to close the window.
       */
      bool shouldClose() { return glfwWindowShouldClose(window); };
      VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

      /**
       * @brief Checks if the window should resized.
       * @return if the window was resize or not.
       */
      bool wasVulkanWindowResized() { return frameBufferResized; }
      void resetVulkanWindowResizedFlag() { frameBufferResized = false; };

      /**
       * @brief Creates a Vulkan drawable surface associated with the window.
       *
       * This function creates a `VkSurfaceKHR` object, which is an abstraction
       * that connects Vulkan to the underlying window system (e.g., GLFW, Win32, Xlib).
       * The created surface is an instance-level object.
       *
       * @param instance The Vulkan instance with which to associate the surface.
       * VK_KHR_win32_surface, VK_KHR_xcb_surface, or VK_KHR_wayland_surface).
       * @param[out] surface A pointer to a `VkSurfaceKHR` handle where the created
       * surface object will be stored. This value will be VK_NULL_HANDLE
       * if the creation fails.
       *
       */
      void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

      /**
       * @brief Polls OS events for the window.
       */
      void pollEvents();

      /**
       * @brief Returns the raw GLFW window pointer.
       * @return A pointer to the GLFWwindow.
       */
      GLFWwindow* getNativeHandle() const;

    private:
      static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
      void initWindow();

      GLFWwindow* window;
      int width;
      int height;
      bool frameBufferResized = false;

      std::string windowName;
    };
  } // namespace Platform
} // namespace GameEngine
