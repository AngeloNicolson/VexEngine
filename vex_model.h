#pragma once

#include "vex_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace vex {
	class VexModel {
	public:
		struct Vertex {
			glm::vec2 position;
			// Combining color to position vertex buffer
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		VexModel(VexDevice& device, const std::vector<Vertex>& vertices);
		~VexModel();

		VexModel(const VexModel&) = delete;
		VexModel& operator=(const VexModel&) = delete;

		// Implement this in vex_model.cpp
		void bind(VkCommandBuffer commandbuffer);
		void draw(VkCommandBuffer commandbuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		VexDevice& vexDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}
