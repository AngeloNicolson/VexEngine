#pragma once

#include "vex_window.h"
#include "vex_device.h"
#include "vex_renderer.h"
#include "vex_game_object.h"
#include "vex_descriptors.h"

//std
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
		void loadGameObjects();

		VexWindow vexWindow{ WIDTH, HEIGHT, "VEXOID" };
		VexDevice vexDevice{ vexWindow };
		VexRenderer vexRenderer{ vexWindow, vexDevice };

		// Order of these declarations is important
		std::unique_ptr<VexDescriptorPool> globalPool{};
		std::vector<VexGameObject> gameObjects;
	};
}