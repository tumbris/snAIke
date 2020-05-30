#include <snAIke/SnakeGame/Controllers/ImGuiSnakeController.hpp>
#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <imgui.h>

ImGuiSnakeController::ImGuiSnakeController()
    : direction(Direction::Up)
{
}

ImGuiSnakeController::~ImGuiSnakeController()
{
    if (auto director = Singleton<Director>::GetInstance())
    {
        for (auto id : callbacks)
        {
            director->RemoveUpdatorCallback(id);
        }
    }
}

Direction ImGuiSnakeController::GetDirection()
{
    return direction;
}

void ImGuiSnakeController::ImGuiUpdate_Impl()
{

    static constexpr float buttonSize = 60;
    auto windowSize = ImGui::GetWindowContentRegionWidth();

    ImGui::Indent((windowSize / 2) - buttonSize / 2);
    if (ImGui::Button("Up", { buttonSize, buttonSize }))
    {
        direction = Direction::Up;
    }
    ImGui::Unindent((windowSize / 2) - buttonSize / 2);

    ImGui::Indent((windowSize / 2) - buttonSize * 1.5f);
    if (ImGui::Button("Left", { buttonSize, buttonSize }))
    {
        direction = Direction::Left;
    }
    ImGui::SameLine();

    ImGui::Indent(buttonSize * 2.f);
    if (ImGui::Button("Right", { buttonSize, buttonSize }))
    {
        direction = Direction::Right;
    }
    ImGui::Unindent((windowSize / 2) - buttonSize * 1.5f + buttonSize * 2);

    ImGui::Indent((windowSize / 2) - buttonSize / 2);
    if (ImGui::Button("Down", { buttonSize, buttonSize }))
    {
        direction = Direction::Down;
    }
    ImGui::Unindent();

}
