#include "first_app.h"

#include <stdexcept>
#include <array>

namespace vex {
	FirstApp::FirstApp() {
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(vexDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run() {
		while (!vexWindow.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
	}
	void FirstApp::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(vexDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void FirstApp::createPipeline() {
		auto pipelineConfig = VexPipeline::defaultPipelineConfigInfo(vexSwapChain.width(), vexSwapChain.height());
		pipelineConfig.renderPass = vexSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vexPipeline = std::make_unique<VexPipeline>(vexDevice, "vert.spv", "frag.spv", pipelineConfig);
	}

	void FirstApp::createCommandBuffers() {
		commandBuffers.resize(vexSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vexDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vexDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("recording command buffer failed");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vexSwapChain.getRenderPass();
			renderPassInfo.framebuffer = vexSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = vexSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vexPipeline->bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("record command buffer failed");
			}
		}
	}
	void FirstApp::drawFrame() {
		uint32_t imageIndex;
		auto result = vexSwapChain.acquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		result = vexSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	}
}