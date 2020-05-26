#pragma once

#include <snAIke/Singletons/Director/Updator.hpp>
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
    ImGuiRenderWindow(const sf::Vector2u size, const char* lable);
    ~ImGuiRenderWindow();

    void Init(const sf::Vector2u size, const char* label);

    void ImGuiRender();

    void Draw(const sf::Drawable& drawable, sf::RenderStates states = sf::RenderStates::Default);

    void Show() { isOpen = true; }
    void Hide() { isOpen = false; }

    bool IsOpen() const { return isOpen; }

    const ImVec2& GetSize() const;
    void Resize(const ImVec2& size);

protected:
    DirtyValue<ImVec2> size;
    ImVec2 pos{};
private:
    std::string label;
    std::vector<UpdatorIdType> callbacks;

    bool isOpen = true;
    
#if defined(DEBUG)
private:
    void DebugDraw(float);
#endif
};