#include "src/GUI/AppWindow.h"
#include "src/GUI/components/DragAndDrop/DragAndDrop.h"

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

	//drag and drop
	glfwSetWindowUserPointer(window_, this);
	glfwSetDropCallback(window_, dropCallback);

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

void AppWindow::dropCallback(GLFWwindow* window, int count, const char** paths)
{
	if (count <= 0 || paths == nullptr) return;

	auto* app = static_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	if (app) {
		std::filesystem::path droppedPath(reinterpret_cast<const char8_t*>(paths[0]));
		app->handleDroppedOrSelectedFile(droppedPath);
	}
}

void AppWindow::handleUploadedFile(const std::filesystem::path& path)
{
	std::string ext = path.extension().string();
	for (char& c : ext) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

	if (ext != ".osr") {
		selectedReplayPath_.reset();
		statusMessage_ = "Nieprawid³owy format (" + ext + "). Wybierz plik .osr.";
		return;
	}

	selectedReplayPath_ = path;
	statusMessage_.clear();
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
		// Rozci¹gniêcie panelu ImGui na ca³e okno GLFW
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);

		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse;



		ImGui::Begin("ROsu Main", nullptr, windowFlags);

		// Drag And Drop
		if (auto chosenPath = gui::renderDropZone(); chosenPath.has_value())
		{
			handleUploadedFile(*chosenPath);
		}

		ImGui::Spacing();

		// Informacja o wybranym pliku lub b³êdzie
		if (!statusMessage_.empty()) {
			ImGui::TextColored(ImVec4(0.95f, 0.35f, 0.35f, 1.0f), "%s", statusMessage_.c_str());
		}
		else if (selectedReplayPath_.has_value()) {
			auto u8Name = selectedReplayPath_->filename().u8string();
			std::string fileName(reinterpret_cast<const char*>(u8Name.c_str()));
			ImGui::Text("Za³adowano: %s", fileName.c_str());
		}
		else {
			ImGui::TextDisabled("Brak wybranego pliku.");
		}

		ImGui::End();



		// clear and draw
		glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// draw new fram
		glfwSwapBuffers(window_);
	}
}