#include "mesh.hpp"

// std
#include <cassert>
#include <cstring>

namespace GameEngine
{
  namespace Graphics
  {
    Mesh::Mesh(VulkanDevice& device, const std::vector<Vertex>& vertices) : vulkanDevice{device}
    {
      createVertexBuffers(vertices);
    };

    Mesh::~Mesh()
    {
      vkDestroyBuffer(vulkanDevice.device(), vertexBuffer, nullptr);
      vkFreeMemory(vulkanDevice.device(), vertexBufferMemory, nullptr);
    }

    void Mesh::createVertexBuffers(const std::vector<Vertex>& vertices)
    {
      vertexCount = static_cast<uint32_t>(vertices.size());
      assert(vertexCount >= 3 && "Vertex count must be at least 3");
      // This gives the the total amount of bytes required for the vertex buffer to store all the vertices of the model
      VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
      vulkanDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                vertexBuffer, vertexBufferMemory);
      void* data;
      vkMapMemory(vulkanDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
      // Take the vertices data and copy it to the Host mapped memory regeon (CPU)
      memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
      vkUnmapMemory(vulkanDevice.device(), vertexBufferMemory);
    }

    void Mesh::draw(VkCommandBuffer commandBuffer) { vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0); }

    void Mesh::bind(VkCommandBuffer commandBuffer)
    {
      VkBuffer buffers[] = {vertexBuffer};
      VkDeviceSize offsets[] = {0};
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> Mesh::Vertex::getBindingDescriptions()
    {
      std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
      bindingDescriptions[0].binding = 0;
      bindingDescriptions[0].stride = sizeof(Vertex);
      bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Mesh::Vertex::getAttributeDescriptions()
    {
      std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

      // Interleaving position and color together
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, position); // Calculate offset of position mem in Vertex struct

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color); // Calculate offset of color member in Vertex struct

      return attributeDescriptions;
    }

  } // namespace Graphics

} // namespace GameEngine
