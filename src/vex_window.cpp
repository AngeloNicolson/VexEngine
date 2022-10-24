#include "vex_window.h"

#include <stdexcept>
namespace vex {
	VexWindow::VexWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	VexWindow::~VexWindow() {
		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void VexWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void VexWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Window surface failed to create");
		}
	}
	void VexWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto vexWindow = reinterpret_cast<VexWindow*>(glfwGetWindowUserPointer(window));
		vexWindow->framebufferResized = true;
		vexWindow->width = width;
		vexWindow->height = height;
	}
}