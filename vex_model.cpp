#include "vex_model.h"

#include <cassert>
#include <cstring>

namespace vex {
	VexModel::VexModel(VexDevice& device, const std::vector<Vertex>& vertices) : vexDevice{ device } {
		createVertexBuffers(vertices);
	}
	VexModel::~VexModel() {
		vkDestroyBuffer(vexDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(vexDevice.device(), vertexBufferMemory, nullptr);
	}

	void VexModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		vexDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer, vertexBufferMemory);

		void* data;
		vkMapMemory(vexDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(vexDevice.device(), vertexBufferMemory);
	}

	void VexModel::draw(VkCommandBuffer commandBuffer) {
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void VexModel::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	std::vector<VkVertexInputBindingDescription> VexModel::Vertex::getBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	/*Could also write this as:
	std::vector<VkVertexInputAttributeDescription> VexModel::Vertex::getAttributeDescriptions() {
	return {
	{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)},
	{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}}
	}*/
	std::vector<VkVertexInputAttributeDescription> VexModel::Vertex::getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		//implemented interleaving color attribute with color in existing binding
		//May change this latter to have to have seperate binding for color attribute
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return attributeDescriptions;
	}
}