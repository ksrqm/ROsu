#pragma once
#include "src/core/ReplayHeader.h"

#include <filesystem>
#include <optional>
#include <string>

struct GLFWwindow;

class AppWindow
{
public:
    AppWindow(int width, int height, const char* title);
    ~AppWindow();

    void run();

    AppWindow(const AppWindow&) = delete;
    AppWindow& operator=(const AppWindow&) = delete;

private:
    GLFWwindow* window_ = nullptr;

    std::optional<std::filesystem::path> selectedReplayPath_;
    std::string statusMessage_;
    std::optional<ReplayHeader> header_;

    void handleUploadedFile(const std::filesystem::path& path);
    static void dropCallback(GLFWwindow* window, int count, const char** paths);
};