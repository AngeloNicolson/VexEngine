#include "first_app.h"

#include <stdexcept>
#include <array>

namespace vex {
	FirstApp::FirstApp() {
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
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

		vkDeviceWaitIdle(vexDevice.device());
	}

	void FirstApp::loadModels() {
		std::vector<VexModel::Vertex> vertices{
			{{0.0f, -0.5f }, {1.0f, 0.0f, 0.0f}},
			{ {0.5f, 0.5f }, {0.0f, 1.0f, 0.0f}},
			{ {-0.5f, 0.5f }, {0.0f, 0.0f, 1.0f}}
		};
		vexModel = std::make_unique<VexModel>(vexDevice, vertices);
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
		PipelineConfigInfo pipelineConfig{};
		VexPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = vexSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vexPipeline = std::make_unique<VexPipeline>(vexDevice, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig);
	}

	void FirstApp::recreateSwapChain() {
		auto extent = vexWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = vexWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(vexDevice.device());
		// Delete lveSwapChain = nullptr; when I recreat the updated swapChain.
		vexSwapChain = nullptr;
		vexSwapChain = std::make_unique<VexSwapChain>(vexDevice, extent);
		createPipeline();
	}

	void FirstApp::createCommandBuffers() {
		commandBuffers.resize(vexSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vexDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vexDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
	}

	void FirstApp::recordCommandBuffer(int imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("recording command buffer failed");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vexSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vexSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vexSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vexSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vexSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vexSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		vexPipeline->bind(commandBuffers[imageIndex]);
		vexModel->bind(commandBuffers[imageIndex]);
		vexModel->draw(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("record command buffer failed");
		}
	}
	void FirstApp::drawFrame() {
		uint32_t imageIndex;
		auto result = vexSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}
		recordCommandBuffer(imageIndex);
		result = vexSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			vexWindow.wasWindowResized()) {
			vexWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	}
}