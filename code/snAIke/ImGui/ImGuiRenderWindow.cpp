#include <snAIke/ImGui/ImGuiRenderWindow.hpp>

#include <imgui.h>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

constexpr static float INIT_SIZE = 10.f;

#define EXPAND_L1(x) #x
#define EXPAND_L2(x) EXPAND_L1(x)
#define LOCATION __FILE__ " : " EXPAND_L2(__LINE__)

#define EMPTY_LABEL() "EMPTY_LABEL" LOCATION

ImGuiRenderWindow::ImGuiRenderWindow()
    : label(EMPTY_LABEL())
    , size(ImVec2(INIT_SIZE, INIT_SIZE))
{
}

ImGuiRenderWindow::ImGuiRenderWindow(const char* label)
    : label(label)
    , size(ImVec2(INIT_SIZE, INIT_SIZE))
{
    create(static_cast<unsigned int>(INIT_SIZE), static_cast<unsigned int>(INIT_SIZE));
    clear();
}

void ImGuiRenderWindow::Init(const char* label)
{
    this->label = label;

    create(static_cast<unsigned int>(INIT_SIZE), static_cast<unsigned int>(INIT_SIZE));
    clear();
}

void ImGuiRenderWindow::ImGuiRender()
{
    if (isOpen)
    {
        ImGui::Begin(label.c_str(), &isOpen);

        auto windowSize = ImGui::GetWindowSize();
        auto windowPos = ImGui::GetWindowPos();
        size.Set(windowSize);

        if (size.IsDirty())
        {
            auto newSize = size.Get();
            sf::FloatRect newViewRect{
                0.f, 0.f,
                newSize.x,
                newSize.y
            };

            create(static_cast<unsigned int>(newSize.x), static_cast<unsigned int>(newSize.y));

            setView(sf::View(newViewRect));
            size.Reset();
        }

        auto id = getTexture().getNativeHandle();
        ImGui::GetWindowDrawList()->AddImage(id, windowPos, windowPos + windowSize, { 0, 1 }, {1, 0});

        ImGui::End();
    }
}
