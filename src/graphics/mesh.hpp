#pragma once

#include "vulkan_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace GameEngine
{
  namespace Graphics
  {
    /**
     * @brief A class representing a mesh for rendering in Vulkan.
     *
     * The Mesh class manages vertex data and associated Vulkan buffers for rendering.
     */
    class Mesh
    {
    public:
      struct Vertex
      {
        glm::vec2 position; ///< 2D position of the vertex.
        glm::vec3 color;

        /**
         * @brief Retrieves the Vulkan vertex input binding descriptions.
         * @return A vector of VkVertexInputBindingDescription objects.
         */
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

        /**
         * @brief Retrieves the Vulkan vertex input attribute descriptions.
         * @return A vector of VkVertexInputAttributeDescription objects.
         */
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
      };

      /**
       * @brief Constructs a Mesh with the given vertices and Vulkan device.
       * @param device Reference to the VulkanDevice used for buffer creation.
       * @param vertices Vector of Vertex objects containing mesh data.
       */
      Mesh(VulkanDevice& device, const std::vector<Vertex>& vertices);

      ~Mesh();

      // Delete copy constructors because mesh manages Vulkan buffer and memory objects
      Mesh(const Mesh&) = delete;
      Mesh& operator=(const Mesh&) = delete;

      /**
       * @brief Binds the mesh's vertex buffer to the provided command buffer.
       * @param commandBuffer The Vulkan command buffer to bind the mesh to.
       */
      void bind(VkCommandBuffer commandBuffer);

      /**
       * @brief Issues draw commands for the mesh using the provided command buffer.
       * @param commandBuffer The Vulkan command buffer to record draw commands.
       */
      void draw(VkCommandBuffer commandBuffer);

    private:
      /**
       * @brief Creates vertex buffers for the provided vertices.
       * @param vertices Vector of Vertex objects to create buffers for.
       */
      void createVertexBuffers(const std::vector<Vertex>& vertices);

      VulkanDevice& vulkanDevice;        ///< Reference to the Vulkan device.
      VkBuffer vertexBuffer;             ///< Vulkan buffer for vertex data.
      VkDeviceMemory vertexBufferMemory; ///< Vulkan memory for the vertex buffer.
      uint32_t vertexCount;              ///< Number of vertices in the mesh.
    };
  } // namespace Graphics

} // namespace GameEngine
