#include "src/GUI/AppWindow.h"

#include <GLFW/glfw3.h>
#include <stdexcept>


AppWindow::AppWindow(int width, int height, const char* title)
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window_) {
		glfwTerminate();
		throw std::runtime_error("Failed to make a window");
	}

	// glfwSetWindowSizeLimits(window_, 520, 360, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1);

	if (window_ != nullptr) {
		glfwDestroyWindow(window_);
		glfwTerminate();
	}
}

void AppWindow::run()
{
	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();
		glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window_);
	}
}