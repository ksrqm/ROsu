#include "src/GUI/AppWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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
		throw std::runtime_error("Failed to create GLFW window");
	}

	// glfwSetWindowSizeLimits(window_, 520, 360, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1);

	// ImGui inicalization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window_, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

AppWindow::~AppWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (window_ != nullptr) {
		glfwDestroyWindow(window_);
		window_ = nullptr;
	}
	glfwTerminate();
}

void AppWindow::run()
{
	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();

		// start new fram
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// demo
		ImGui::ShowDemoWindow();

		// clear and draw
		glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// draw new fram
		glfwSwapBuffers(window_);
	}
}