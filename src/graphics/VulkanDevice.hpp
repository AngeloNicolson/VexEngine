#pragma once

#include "../platform/Window.hpp"

// std lib headers
// #include <string>
#include <vector>

namespace GameEngine
{
  namespace Graphics
  {

    struct SwapChainSupportDetails
    {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices
    {
      uint32_t graphicsFamily;
      uint32_t presentFamily;
      bool graphicsFamilyHasValue = false;
      bool presentFamilyHasValue = false;
      bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    class VulkanDevice
    {
    public:
#ifdef NDEBUG
      const bool enableValidationLayers = false;
#else
      const bool enableValidationLayers = true;
#endif

      VulkanDevice(Platform::Window& window);
      ~VulkanDevice();

      // Not copyable or movable
      VulkanDevice(const VulkanDevice&) = delete;
      VulkanDevice operator=(const VulkanDevice&) = delete;
      VulkanDevice(VulkanDevice&&) = delete;
      VulkanDevice& operator=(VulkanDevice&&) = delete;

      VkCommandPool getCommandPool() { return commandPool; }
      VkDevice device() { return device_; }
      VkSurfaceKHR surface() { return surface_; }
      VkQueue graphicsQueue() { return graphicsQueue_; }
      VkQueue presentQueue() { return presentQueue_; }

      SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
      uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
      VkFormat
      findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

      /**
       * @brief Creates a Vulkan buffer with specified size, usage, and memory properties.
       * @param size Size of the buffer in bytes.
       * @param usage Vulkan buffer usage flags (e.g., VK_BUFFER_USAGE_VERTEX_BUFFER_BIT).
       * @param properties Vulkan memory property flags (e.g., VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT).
       * @param buffer Reference to the created Vulkan buffer handle.
       * @param bufferMemory Reference to the allocated Vulkan device memory for the buffer.
       */

      void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                        VkDeviceMemory& bufferMemory);

      /**
       * @brief Begins a single-time-use Vulkan command buffer for one-off operations.
       * @return The allocated VkCommandBuffer handle.
       */
      VkCommandBuffer beginSingleTimeCommands();

      /**
       * @brief Ends and submits a single-time-use Vulkan command buffer.
       * @param commandBuffer The VkCommandBuffer to end and submit.
       */
      void endSingleTimeCommands(VkCommandBuffer commandBuffer);

      /**
       * @brief Copies data from a source Vulkan buffer to a destination buffer.
       * @param srcBuffer The source VkBuffer to copy from.
       * @param dstBuffer The destination VkBuffer to copy to.
       * @param size Size of the data to copy in bytes.
       */
      void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

      /**
       * @brief Copies data from a Vulkan buffer to a Vulkan image.
       * @param buffer The source VkBuffer containing the data.
       * @param image The destination VkImage to copy to.
       * @param width Width of the image in pixels.
       * @param height Height of the image in pixels.
       * @param layerCount Number of layers in the image.
       */
      void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

      /**
       * @brief Creates a Vulkan image with the specified creation info and memory properties.
       * @param imageInfo Vulkan image creation info structure.
       * @param properties Vulkan memory property flags (e.g., VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT).
       * @param image Reference to the created VkImage handle.
       * @param imageMemory Reference to the allocated Vulkan device memory for the image.
       */
      void createImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image,
                               VkDeviceMemory& imageMemory);

      /**
       * @brief Stores properties of the physical Vulkan device.
       */
      VkPhysicalDeviceProperties properties;

    private:
      void createInstance();
      void setupDebugMessenger();
      void createSurface();
      void pickPhysicalDevice();
      void createLogicalDevice();
      void createCommandPool();

      // helper functions
      bool isDeviceSuitable(VkPhysicalDevice device);
      std::vector<const char*> getRequiredExtensions();
      bool checkValidationLayerSupport();
      QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
      void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
      void hasGflwRequiredInstanceExtensions();
      bool checkDeviceExtensionSupport(VkPhysicalDevice device);
      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

      VkInstance instance;
      VkDebugUtilsMessengerEXT debugMessenger;
      VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
      GameEngine::Platform::Window& window;
      VkCommandPool commandPool;

      VkDevice device_;
      VkSurfaceKHR surface_;
      VkQueue graphicsQueue_;
      VkQueue presentQueue_;

      const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
      const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    };
  } // namespace Graphics
} // namespace GameEngine
