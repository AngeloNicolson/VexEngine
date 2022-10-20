#pragma once

#include "vex_window.h"
#include "vex_device.h"
#include "vex_swap_chain.h"

//std
#include <cassert>
#include <memory>
#include <vector>

namespace vex {
	class VexRenderer {
	public:
		VexRenderer(VexWindow& window, VexDevice& device);
		~VexRenderer();

		VexRenderer(const VexRenderer&) = delete;
		VexRenderer& operator = (const VexRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return vexSwapChain->getRenderPass(); }
		float getAspectRataio() const { return vexSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get Command Buffer when frame is not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get Command Buffer when frame is not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		VexWindow& vexWindow;
		VexDevice& vexDevice;
		std::unique_ptr<VexSwapChain> vexSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted{ false };
	};
}