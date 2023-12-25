//===-- myth_vertex_buffer_manager.hpp ------------------------------------===//
//
// Part of the Myth Engine, a Vulkan-based graphics engine.
//
// This file defines the MythVertexBufferManager class, responsible for
// managing Vulkan vertex buffers and associated memory.
//
//===----------------------------------------------------------------------===//
#ifndef MYTH_VERTEX_BUFFER_MANAGER_H
#define MYTH_VERTEX_BUFFER_MANAGER_H

#include "myth_engine_device.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace myth_engine {
class MythVertexBufferManager {
public:
  struct Vertex {
    glm::vec2 position;

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

  MythVertexBufferManager(MythEngineDevice &device,
                          const std::vector<Vertex> &vertices);
  ~MythVertexBufferManager();

  // make sure that object cannot be copied or allocated
  MythVertexBufferManager(const MythVertexBufferManager &) = delete;
  MythVertexBufferManager &operator=(const MythVertexBufferManager &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);

  MythEngineDevice &mythDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
};

} // namespace myth_engine

#endif
