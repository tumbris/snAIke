#pragma once

#include <memory>
#include <cstdint>

namespace sf
{
    class RenderWindow;
}

class Application
{
public:
    void Init(std::uint32_t width, std::uint32_t height, const char* name);
    int Run();
private:
    void Deinit();

    void DisplayDockSpace();
    void DisplayMenuBar();

    void PollEvents(sf::RenderWindow& window);
private:
    struct ApplicationImpl;
    ApplicationImpl* impl = nullptr;
};