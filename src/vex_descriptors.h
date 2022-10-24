#pragma once

#include "vex_device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace vex {
	class VexDescriptorSetLayout {
	public:
		class Builder {
		public:
			Builder(VexDevice& vexDevice) : vexDevice{ vexDevice } {}

			Builder& addBinding(
				uint32_t binding,
				VkDescriptorType descriptorType,
				VkShaderStageFlags stageFlags,
				uint32_t count = 1);
			std::unique_ptr<VexDescriptorSetLayout> build() const;

		private:
			VexDevice& vexDevice;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
		};

		VexDescriptorSetLayout(
			VexDevice& vexDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~VexDescriptorSetLayout();
		VexDescriptorSetLayout(const VexDescriptorSetLayout&) = delete;
		VexDescriptorSetLayout& operator=(const VexDescriptorSetLayout&) = delete;

		VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

	private:
		VexDevice& vexDevice;
		VkDescriptorSetLayout descriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

		friend class VexDescriptorWriter;
	};

	class VexDescriptorPool {
	public:
		class Builder {
		public:
			Builder(VexDevice& vexDevice) : vexDevice{ vexDevice } {}

			Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& setMaxSets(uint32_t count);
			std::unique_ptr<VexDescriptorPool> build() const;

		private:
			VexDevice& vexDevice;
			std::vector<VkDescriptorPoolSize> poolSizes{};
			uint32_t maxSets = 1000;
			VkDescriptorPoolCreateFlags poolFlags = 0;
		};

		VexDescriptorPool(
			VexDevice& vexDevice,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const std::vector<VkDescriptorPoolSize>& poolSizes);
		~VexDescriptorPool();
		VexDescriptorPool(const VexDescriptorPool&) = delete;
		VexDescriptorPool& operator=(const VexDescriptorPool&) = delete;

		bool allocateDescriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

		void resetPool();

	private:
		VexDevice& vexDevice;
		VkDescriptorPool descriptorPool;

		friend class VexDescriptorWriter;
	};

	class VexDescriptorWriter {
	public:
		VexDescriptorWriter(VexDescriptorSetLayout& setLayout, VexDescriptorPool& pool);

		VexDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		VexDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool build(VkDescriptorSet& set);
		void overwrite(VkDescriptorSet& set);

	private:
		VexDescriptorSetLayout& setLayout;
		VexDescriptorPool& pool;
		std::vector<VkWriteDescriptorSet> writes;
	};
}  // namespace vex