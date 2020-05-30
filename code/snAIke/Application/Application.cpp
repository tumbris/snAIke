#include <snAIke/Application/Application.hpp>
#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Singletons/GameManager.hpp>

#include <snAIke/SnakeGame/SnakeGame.hpp>
#include <snAIke/ImGui/ImGuiRenderWindow.hpp>
#include <snAIke/Utility/DirtyValue.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>

#include <SFML/Graphics.hpp>

struct Application::ApplicationImpl
{
public:
    void PollEvents();
    void Init(std::uint32_t width, std::uint32_t height, const char* name);
public:
    sf::RenderWindow mainWindow;
    ImGuiRenderWindow gameMainRenderWindow;

    sf::Clock deltaClock;
    sf::Time frameTime;
};

Application::Application() : impl(nullptr)
{
}

Application::~Application()
{
    delete impl;
}

void Application::Init(std::uint32_t width, std::uint32_t height, const char* name)
{
    impl = new ApplicationImpl;
    impl->Init(width, height, name);

    impl->mainWindow.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(impl->mainWindow);
}

int Application::Run()
{
    if (!impl) return EXIT_FAILURE;

    Singleton<GameManager>::GetInstance()->Init();

    auto director = Singleton<Director>::GetInstance();

    while (impl->mainWindow.isOpen())
    {
        BeginFrame();
        director->TriggerUpdates(UpdatePriority::FrameBegin);
        director->TriggerUpdates(UpdatePriority::PostFrameBegin);

        director->TriggerUpdates(UpdatePriority::ImGuiUpdate);
        ImGuiRender();
        director->TriggerUpdates(UpdatePriority::ApplicationUpdate);
        Update();
        director->TriggerUpdates(UpdatePriority::GameUpdate);

        director->TriggerUpdates(UpdatePriority::Default);

        director->TriggerUpdates(UpdatePriority::PreFrameEnd);
        director->TriggerUpdates(UpdatePriority::FrameEnd);
        EndFrame();
    }

    return EXIT_SUCCESS;
}

float Application::GetFrameTime() const
{
    return impl->frameTime.asSeconds();
}

ImGuiRenderWindow* Application::GetGameRenderWindow()
{
    assert(impl != nullptr);
    return &(impl->gameMainRenderWindow);
}

sf::RenderWindow* Application::GetApplicationRenderWindow()
{
    return &(impl->mainWindow);
}

void Application::BeginFrame()
{
    impl->PollEvents();
    impl->frameTime = impl->deltaClock.restart();
    ImGui::SFML::Update(impl->mainWindow, impl->frameTime);
    DisplayDockSpace();
    impl->mainWindow.clear();
    impl->gameMainRenderWindow.clear();
}

void Application::Update()
{

}

void Application::EndFrame()
{
    impl->mainWindow.display();
}

void Application::ImGuiRender()
{
    impl->gameMainRenderWindow.display();
    impl->gameMainRenderWindow.ImGuiRender();

    ImGui::SFML::Render(impl->mainWindow);
}

void Application::DisplayDockSpace()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("main_dockspace_window", nullptr, window_flags);
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
            if (ImGui::MenuItem("Veiwport", "", impl->gameMainRenderWindow.IsOpen()))
            {
                if (impl->gameMainRenderWindow.IsOpen()) impl->gameMainRenderWindow.Hide();
                else impl->gameMainRenderWindow.Show();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Application::ApplicationImpl::PollEvents()
{
    sf::Event event;
    while (mainWindow.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);

        switch (event.type)
        {
        case sf::Event::Closed: mainWindow.close(); break;
        case sf::Event::Resized:
        {
            sf::FloatRect newViewport{
                0.f, 0.f,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            };

            mainWindow.setView(sf::View(newViewport));
            break;
        }
        default: break;
        }
    }
}

void Application::ApplicationImpl::Init(std::uint32_t width, std::uint32_t height, const char* name)
{
    sf::VideoMode videoMode(width, height);
    mainWindow.create(videoMode, name);
    gameMainRenderWindow.Init({ width, height }, "Viewport");
}