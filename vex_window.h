#pragma once

#define GLFW_INCLUDE_VULkAN
#include <GLFW/glfw3.h>

#include <string>
namespace vex {
	class VexWindow {
	public:
		VexWindow(int w, int h, std::string name);
		~VexWindow();

		bool shouldClose() {
			return glfwWindowShouldClose(window);
		}

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}