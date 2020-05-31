#pragma once

#include <snAIke/SnakeGame/Controllers/SnakeController.hpp>
#include <snAIke/AI/NeuralNetwork/FeedForwardNN.hpp>
#include <snAIke/Singletons/Director/Types.hpp>
#include <snAIke/Utility/Ref/IntrusivePtr.hpp>
#include <array>

class AISnakeController : public SnakeController
{
public:
    enum LookDirection : unsigned long
    {
        Up = 1 << 0,
        Down = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        UpRight = Up | Right,
        UpLeft = Up | Left,
        DownRight = Down | Right,
        DownLeft = Down | Left
    };
public:
    
    void Init(const IntrusivePtr<FeedForwardNN>& network);

    virtual Direction GetDirection() override;
    virtual const char* GetName() const override { return "AISnakeController"; }

protected:
    virtual void ImGuiUpdate_Impl() override;
    std::vector<float> CalculateInputs() const;

    std::size_t GetFoodDistanse(LookDirection dir) const;
    std::size_t GetTailDistanse(LookDirection dir) const;
    std::size_t GetWallDistanse(LookDirection dir) const;

    std::array<float, 3> LookInDirection(std::int64_t x, std::int64_t y) const;

private:
    IntrusivePtr<FeedForwardNN> network;
    Direction direction = Direction::Up;
};

