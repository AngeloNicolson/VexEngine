#include "myth_vertex_buffer_manager.hpp"
#include "myth_engine_device.hpp"
#include <cstddef>
#include <vulkan/vulkan_core.h>

// std
#include <cstdint>
#include <cstring>

namespace myth_engine {

MythVertexBufferManager::MythVertexBufferManager(
    MythEngineDevice &device, const std::vector<Vertex> &vertices)
    : mythDevice{device} {
  createVertexBuffers(vertices);
}

MythVertexBufferManager::~MythVertexBufferManager() {
  vkDestroyBuffer(mythDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(mythDevice.device(), vertexBufferMemory, nullptr);
}

void MythVertexBufferManager::createVertexBuffers(
    const std::vector<Vertex> &vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

  mythDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          vertexBuffer, vertexBufferMemory);

  void *data;
  vkMapMemory(mythDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<uint32_t>(bufferSize));
  vkUnmapMemory(mythDevice.device(), vertexBufferMemory);
}

void MythVertexBufferManager::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void MythVertexBufferManager::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription>
MythVertexBufferManager::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
MythVertexBufferManager::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);

  // Interleaving the color in the buffer
  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
  return attributeDescriptions;
}
} // namespace myth_engine
