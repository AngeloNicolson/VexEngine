#pragma once

#include "vex_window.h"
#include "vex_pipeline.h"
#include "vex_device.h"

namespace vex {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		VexWindow vexWindow{ WIDTH, HEIGHT, "VEXOID" };
		VexDevice vexDevice{ vexWindow };
		VexPipeline vexPipeline{
			vexDevice,
			"vert.spv",
			"frag.spv",
		VexPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT) };
	};
}