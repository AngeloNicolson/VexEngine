#include "first_app.h"

namespace vex {
	void FirstApp::run() {
		while (!vexWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}