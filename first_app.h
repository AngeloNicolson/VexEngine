#pragma once

#include "vex_window.h"
#include "vex_pipeline.h"

namespace vex {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		VexWindow vexWindow{ WIDTH, HEIGHT, "VEXOID" };
		// For some reason i cant point the files to the shaders folder
		VexPipeline vexPipeline{ "vert.spv", "frag.spv" };
	};
}