#include "vex_renderer.h"
//std
#include <stdexcept>
#include <array>
#include <cassert>

namespace vex {
	VexRenderer::VexRenderer(VexWindow& window, VexDevice& device) : vexWindow{ window }, vexDevice{ device } {
		recreateSwapChain();
		createCommandBuffers();
	}
	// This allows the renderer to be destroyed but Application continues to run
	VexRenderer::~VexRenderer() { freeCommandBuffers(); }

	void VexRenderer::recreateSwapChain() {
		auto extent = vexWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = vexWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(vexDevice.device());
		if (vexSwapChain == nullptr) {
			vexSwapChain = std::make_unique<VexSwapChain>(vexDevice, extent);
		}
		else {
			std::shared_ptr<VexSwapChain> oldSwapChain = std::move(vexSwapChain);
			vexSwapChain = std::make_unique<VexSwapChain>(vexDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapChainFormats(*vexSwapChain.get())) {
				throw std::runtime_error("SwapChain image or depth format was changed");
				// Need to set up a callback funtion to let the app know an incompatable render pass has been created.
			}
		}
	}

	void VexRenderer::createCommandBuffers() {
		commandBuffers.resize(VexSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vexDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vexDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
	}

	void VexRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(
			vexDevice.device(),
			vexDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer VexRenderer::beginFrame() {
		assert(!isFrameStarted && "Cant Call beginFrame while already running");
		auto result = vexSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("recording command buffer failed");
		}
		return commandBuffer;
	}
	void VexRenderer::endFrame() {
		assert(isFrameStarted && "Cant endFrame while Currentframe is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("record command buffer failed");
		}
		auto result = vexSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			vexWindow.wasWindowResized()) {
			vexWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % VexSwapChain::MAX_FRAMES_IN_FLIGHT;
	}
	void VexRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "Cant call beginSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cant Begin renderpass on cammand buffer from a different frame");
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vexSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vexSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vexSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vexSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vexSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vexSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	void VexRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "Cant call endSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cant end renderpass on command buffer from a different frame");
		vkCmdEndRenderPass(commandBuffer);
	}
}