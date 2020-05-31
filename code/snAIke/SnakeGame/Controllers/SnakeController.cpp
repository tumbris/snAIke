#include <snAIke/SnakeGame/Controllers/SnakeController.hpp>

#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <imgui.h>

SnakeController::SnakeController()
{
    //if (auto director = Singleton<Director>::GetInstance())
    //{
    //    callbacks.push_back(director->AddUpdatorCallback(Callback(this, &SnakeController::Update), UpdatePriority::GameUpdate));
    //}
}

SnakeController::~SnakeController()
{
    //if (auto director = Singleton<Director>::GetInstance())
    //{
    //    for (auto id : callbacks)
    //    {
    //        director->RemoveUpdatorCallback(id);
    //    }
    //}
}

void SnakeController::Bind(const TileGrid& grid)
{
    field = &grid;
}

void SnakeController::Bind(const Snake& snake)
{
    this->snake = &snake;
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

const Snake& SnakeController::GetSnake() const
{
    return *snake;
}

Fruit SnakeController::GetFruit() const
{
    for (std::size_t i = 0; i < field->size(); ++i)
    {
        for (std::size_t j = 0; j < (*field)[i].size(); ++j)
        {
            if ((*field)[i][j] == TileType_Fruit)
            {
                return { std::int64_t(i), std::int64_t(j) };
            }
        }
    }
    return { -1, -1 };
}
