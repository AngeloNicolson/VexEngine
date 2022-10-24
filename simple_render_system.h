#pragma once

#include "vex_camera.h"
#include "vex_pipeline.h"
#include "vex_device.h"
#include "vex_game_object.h"
#include "vex_frame_info.h"

//std
#include <memory>
#include <vector>

/* The renderPass here will just be used to create the pipeline and we will not store the renderPass as the SimpleRenderSystems lifecycle is not tied to the render passes */
namespace vex {
	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(VexDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator = (const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		VexDevice& vexDevice;

		std::unique_ptr<VexPipeline> vexPipeline;
		VkPipelineLayout pipelineLayout;
	};
}