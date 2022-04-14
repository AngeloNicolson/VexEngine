#pragma once

#include "vex_window.h"
#include "vex_pipeline.h"
#include "vex_device.h"
#include "vex_swap_chain.h"

#include <memory>
#include <vector>

namespace vex {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator = (const FirstApp&) = delete;
		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		VexWindow vexWindow{ WIDTH, HEIGHT, "VEXOID" };
		VexDevice vexDevice{ vexWindow };
		VexSwapChain vexSwapChain{ vexDevice, vexWindow.getExtent() };
		std::unique_ptr<VexPipeline> vexPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}