#include <snAIke/ImGui/ImGuiRenderWindow.hpp>

#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Application/Application.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
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

ImGuiRenderWindow::ImGuiRenderWindow(const sf::Vector2u size, const char* label)
    : label(label)
    , size(ImVec2(static_cast<float>(size.x), static_cast<float>(size.y)))
{
}

ImGuiRenderWindow::~ImGuiRenderWindow()
{
    if (auto director = Singleton<Director>::GetInstance())
    {
        for (auto id : callbacks)
        {
            director->RemoveUpdatorCallback(id);
        }
    }
}

void ImGuiRenderWindow::Init(const sf::Vector2u size, const char* label)
{
    this->label = label;

    this->size.Set(ImVec2(static_cast<float>(size.x), static_cast<float>(size.y)));
    this->size.SetForceDirty();

    create(size.x, size.y);

    if (auto director = Singleton<Director>::GetInstance())
    {
    #if defined(DEBUG)
        callbacks.push_back(director->AddUpdatorCallback(Callback(this, &ImGuiRenderWindow::DebugDraw)));
    #endif
    }
}

void ImGuiRenderWindow::ImGuiRender()
{
    if (isOpen)
    {
        ImGui::Begin(label.c_str(), &isOpen);

        pos = ImGui::GetWindowPos();
        Resize(ImGui::GetWindowSize());

        const auto& texture = getTexture();
        auto id = texture.getNativeHandle();

        auto a = pos;
        auto c = pos + size.Get();

        static const ImVec2 uv_a{ 0, 1 };
        static const ImVec2 uv_c{ 1, 0 };
        ImGui::GetWindowDrawList()->AddImage(id, a, c, uv_a, uv_c);
        ImGui::End();
    }
}

void ImGuiRenderWindow::Draw(const sf::Drawable& drawable, sf::RenderStates states)
{
    draw(drawable, states);
}

const ImVec2& ImGuiRenderWindow::GetSize() const
{
    return size.Get();
}

void ImGuiRenderWindow::Resize(const ImVec2& size)
{
    this->size.Set(size);

    if (this->size.IsDirty())
    {
        sf::FloatRect newViewRect{
            0.f, 0.f,
            size.x,
            size.y
        };
        create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        setView(sf::View(newViewRect));

        this->size.Reset();
    }
}

#if defined(DEBUG)
void ImGuiRenderWindow::DebugDraw(float)
{
    //auto windowSize = size.Get();
    //auto windowPos = pos;

    //auto a = windowPos;
    //auto c = windowPos + windowSize;
    //ImVec2 b(c.x, a.y), d(a.x, c.y);

    //sf::RenderWindow* mainWindow = Singleton<Director>::GetInstance()->GetMainApplication()->GetApplicationRenderWindow();

    //sf::CircleShape s(10);
    //s.setFillColor(sf::Color::Green);

    //s.setPosition(a.x, a.y);
    //mainWindow->draw(s);
    //s.setPosition(b.x, b.y);
    //mainWindow->draw(s);
    //s.setPosition(c.x, c.y);
    //mainWindow->draw(s);
    //s.setPosition(d.x, d.y);
    //mainWindow->draw(s);
}
#endif