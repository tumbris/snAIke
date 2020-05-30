#pragma once

#include <snAike/SnakeGame/Controllers/SnakeController.hpp>
#include <snAIke/Singletons/Director/Types.hpp>

class ImGuiSnakeController : public SnakeController
{
public:
    ImGuiSnakeController();
    ~ImGuiSnakeController();

    virtual Direction GetDirection() override;
    virtual const char* GetName() const override { return "ImGuiSnakeController"; }

private:
    virtual void ImGuiUpdate_Impl() override;

private:
    std::vector<UpdatorIdType> callbacks;
    Direction direction;
};

