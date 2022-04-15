#pragma once

#include "vex_window.h"
#include "vex_pipeline.h"
#include "vex_device.h"
#include "vex_swap_chain.h"
#include "vex_model.h"

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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		VexWindow vexWindow{ WIDTH, HEIGHT, "VEXOID" };
		VexDevice vexDevice{ vexWindow };
		std::unique_ptr<VexSwapChain> vexSwapChain;
		std::unique_ptr<VexPipeline> vexPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<VexModel> vexModel;
	};
}