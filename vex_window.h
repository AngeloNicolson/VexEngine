#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace vex {
	class VexWindow {
	public:
		VexWindow(int w, int h, std::string name);
		~VexWindow();

		/* This is breaking the code somehow??*/
		VexWindow(const VexWindow&) = delete;
		VexWindow& operator = (const VexWindow&) = delete;

		bool shouldClose() {
			return glfwWindowShouldClose(window);
		}

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}