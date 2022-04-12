#pragma once

#include "vex_window.h"

namespace vex {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		VexWindow vexWindow{ WIDTH, HEIGHT, "VEXOID" };
	};
}