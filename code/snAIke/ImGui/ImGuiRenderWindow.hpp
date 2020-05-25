#pragma once

#include <snAIke/Utility/DirtyValue.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <string>

#include <imgui.h>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui_internal.h"

class ImGuiRenderWindow : public sf::RenderTexture
{
public:
    ImGuiRenderWindow();
    ImGuiRenderWindow(const char* lable);

    void Init(const char* label);

    void ImGuiRender();

    void Show() { isOpen = true; }
    void Hide() { isOpen = false; }

    bool IsOpen() const { return isOpen; }

private:
    DirtyValue<ImVec2> size;
    std::string label;
    bool isOpen = false;
};