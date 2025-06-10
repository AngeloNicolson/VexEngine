// Window declaration
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
    class Window
    {
    public:
      /**
       * @brief Constructs the window.
       * @param w The width of the window in pixels.
       * @param h The height of the window in pixels.
       * @param name The title of the window.
       */
      Window(int w, int h, std::string name);

      ~Window();

      /**
       * @brief Deleted copy constructor to prevent copying of Window instances.
       *
       * Copying a Window object is disallowed to avoid issues with
       * resource management and ownership.
       */
      Window(const Window&) = delete;

      /**
       * @brief Deleted copy assignment operator to prevent copying of Window instances.
       *
       * Assigning one Window object to another is disallowed to maintain
       * unique ownership of underlying resources.
       *
       * @return Reference to the current Window object.
       */
      Window& operator=(const Window&) = delete;

      /**
       * @brief Checks if the window should close.
       * @return true if the user has requested to close the window.
       */
      bool shouldClose() { return glfwWindowShouldClose(window); };

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
      void initWindow();

      GLFWwindow* window;
      const int width;
      const int height;
      std::string windowName;
    };
  } // namespace Platform
} // namespace GameEngine
