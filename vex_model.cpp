#include "vex_model.h"

#include <cassert>
#include <cstring>

namespace vex {
	VexModel::VexModel(VexDevice& device, const VexModel::Data& builder) : vexDevice{ device } {
		createVertexBuffers(builder.vertices);
		createIndexBuffer(builder.indices);
	}
	VexModel::~VexModel() {
		vkDestroyBuffer(vexDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(vexDevice.device(), vertexBufferMemory, nullptr);

		if (hasIndexBuffer) {
			vkDestroyBuffer(vexDevice.device(), indexBuffer, nullptr);
			vkFreeMemory(vexDevice.device(), indexBufferMemory, nullptr);
		}
	}

	void VexModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		// Staging buffer which copys content from Host?
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		vexDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(vexDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(vexDevice.device(), stagingBufferMemory);

		vexDevice.createBuffer(bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer, vertexBufferMemory);

		vexDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		// Destructor
		vkDestroyBuffer(vexDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(vexDevice.device(), stagingBufferMemory, nullptr);
	}

	void VexModel::createIndexBuffer(const std::vector<uint32_t>& indices) {
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0;

		if (!hasIndexBuffer) {
			return;
		}

		// Buffer size declaration
		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		vexDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(vexDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(vexDevice.device(), stagingBufferMemory);

		vexDevice.createBuffer(bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer, indexBufferMemory);

		vexDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		// Destructor
		vkDestroyBuffer(vexDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(vexDevice.device(), stagingBufferMemory, nullptr);
	}

	void VexModel::draw(VkCommandBuffer commandBuffer) {
		if (hasIndexBuffer) {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}
	}

	void VexModel::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
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
	{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
	{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}}
	}*/
	std::vector<VkVertexInputAttributeDescription> VexModel::Vertex::getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
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