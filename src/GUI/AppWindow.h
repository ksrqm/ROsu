#pragma once

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
};