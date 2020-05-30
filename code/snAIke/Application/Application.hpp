#pragma once

#include <memory>
#include <cstdint>

class ImGuiRenderWindow;
class SnakeGame;

namespace sf
{
    class RenderWindow;
}

class Application
{
public:
    Application();
    ~Application();

    void Init(std::uint32_t width, std::uint32_t height, const char* name);
    int Run();

    float GetFrameTime() const;

    ImGuiRenderWindow* GetGameRenderWindow();
    sf::RenderWindow* GetApplicationRenderWindow();

private:

    void BeginFrame();
    void Update();
    void EndFrame();

    void ImGuiRender();

    void DisplayDockSpace();
    void DisplayMenuBar();
private:
    struct ApplicationImpl;
    ApplicationImpl* impl = nullptr;
};