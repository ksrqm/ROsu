#include "src/GUI/AppWindow.h"

#include <GLFW/glfw3.h>
#include <stdexcept>


AppWindow::AppWindow(int width, int height, const char* title)
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	if (!glfwCreateWindow(width, height, title, nullptr, nullptr)) {
		glfwTerminate();
		throw std::runtime_error("Failed to make a window");
	}

	// glfwSetWindowSizeLimits(window_, 520, 360, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1);
}