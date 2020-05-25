#include <snAIke/Application/Application.hpp>

#include <snAIke/ImGui/ImGuiRenderWindow.hpp>
#include <snAIke/Singletons/Singletons.hpp>
#include <snAIke/Utility/DirtyValue.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#include <SFML/Graphics.hpp>

struct Application::ApplicationImpl
{
    sf::RenderWindow window;
    ImGuiRenderWindow mainViewport;

    static ApplicationImpl* Create(std::uint32_t width, std::uint32_t height, const char* name);
};

void Application::Init(std::uint32_t width, std::uint32_t height, const char* name)
{
    InitializeSingletons();


    impl = ApplicationImpl::Create(width, height, name);
}

int Application::Run()
{
    if (!impl)
    {
        return EXIT_FAILURE;
    }

    auto& window = impl->window;
    auto& viewport = impl->mainViewport;
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window);

    sf::CircleShape circle(100);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(50.f, 50.f);

    sf::Clock deltaClock;
    sf::Time frameTime = deltaClock.restart();
    while (window.isOpen())
    {
        PollEvents(window);
        frameTime = deltaClock.restart();

        ImGui::SFML::Update(window, frameTime);

        DisplayDockSpace();

        ImGui::ShowDemoWindow();

        viewport.clear();
        viewport.draw(circle);
        viewport.display();

        window.clear();
        viewport.ImGuiRender();
        ImGui::SFML::Render(window);
        window.display();
    }

    Deinit();
    return EXIT_SUCCESS;
}

void Application::Deinit()
{
    DeinitializeSingletons();

    delete impl;
}

void Application::DisplayDockSpace()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    //ImGui::SetNextWindowPos(viewport->GetWorkPos());
    //ImGui::SetNextWindowSize(viewport->GetWorkSize());
    //ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("main_dockspace_window", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("main_dock_space");
    ImGui::DockSpace(dockspace_id);

    DisplayMenuBar();

    ImGui::End();
}

void Application::DisplayMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Veiwport", "", impl->mainViewport.IsOpen()))
            {
                if (impl->mainViewport.IsOpen()) impl->mainViewport.Hide();
                else impl->mainViewport.Show();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Application::PollEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);

        switch (event.type)
        {
        case sf::Event::Closed: window.close(); break;
        case sf::Event::Resized:
        {
            sf::FloatRect newViewport{
                0.f, 0.f,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            };

            window.setView(sf::View(newViewport));
            break;
        }
        default: break;
        }
    }
}

Application::ApplicationImpl* Application::ApplicationImpl::Create(std::uint32_t width, std::uint32_t height, const char* name)
{
    sf::VideoMode videoMode(width, height);
    auto res = new ApplicationImpl;
    res->window.create(videoMode, name);
    res->mainViewport.Init("Viewport");
    return res;
}
