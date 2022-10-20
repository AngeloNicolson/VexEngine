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
			glm::vec3 position;
			// Combining color to position vertex buffer
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		struct Data {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
		};

		VexModel(VexDevice& device, const VexModel::Data& builder);
		~VexModel();

		VexModel(const VexModel&) = delete;
		VexModel& operator=(const VexModel&) = delete;

		void bind(VkCommandBuffer commandbuffer);
		void draw(VkCommandBuffer commandbuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffer(const std::vector<uint32_t>& indices);

		VexDevice& vexDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;
	};
}
