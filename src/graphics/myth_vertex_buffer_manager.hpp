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

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace myth_engine
{
class MythVertexBufferManager
{
  public:
    struct Vertex
    {

        // Interleaving position and color
        glm::vec3 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    MythVertexBufferManager(MythEngineDevice &device, const std::vector<Vertex> &vertices);
    ~MythVertexBufferManager();

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
