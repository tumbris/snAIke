#include <snAIke/SnakeGame/Controllers/SnakeController.hpp>

#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <imgui.h>

SnakeController::SnakeController()
{
    if (auto director = Singleton<Director>::GetInstance())
    {
        callbacks.push_back(director->AddUpdatorCallback(Callback(this, &SnakeController::Update), UpdatePriority::GameUpdate));
    }
}

SnakeController::~SnakeController()
{
    if (auto director = Singleton<Director>::GetInstance())
    {
        for (auto id : callbacks)
        {
            director->RemoveUpdatorCallback(id);
        }
    }
}

void SnakeController::Bind(const TileGrid& grid)
{
    field = &grid;
}

void SnakeController::Update(float dt)
{
    Update_Impl(dt);
}

void SnakeController::ImGuiUpdate(float)
{
    ImGui::BeginChild(GetName());
    ImGuiUpdate_Impl();
    ImGui::EndChild();
}
