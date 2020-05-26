#pragma once

#include <snAike/SnakeGame/SnakeController.hpp>
#include <snAIke/Singletons/Director/Types.hpp>

class ImGuiSnakeController : public SnakeController
{
public:
    ImGuiSnakeController();
    ~ImGuiSnakeController();

    virtual Direction GetDirection(const TileGrid& field) override;

private:
    void ImGuiUpdate(float);

private:
    std::vector<UpdatorIdType> callbacks;
    Direction direction;
};

