#pragma once

#include <snAIke/SnakeGame/Types.hpp>
#include <snAIke/AI/NeuralNetwork/FeedForwardNN.hpp>

#include <snAIke/SnakeGame/Field.hpp>
#include <snAIke/Utility/Ref/IntrusivePtr.hpp>

#include <functional>
#include <vector>

class SnakeController;

class AILearningPlayground
{
private:
    static constexpr std::size_t snakeInitSize = 4;
public:
    void Init(std::size_t fieldSize = 32);

    AISnakeData Run(const std::vector<float>& weights, const std::vector<float>& biases);

private:
    void StartGame();
    void StopGame();

    void SpawnSnake();
    void SpawnFruit();
    void ImGuiRender(float);

    void Tick();

    void UpdateSnakePosition(Direction dir);
    bool CheckBounds();
    void RefreshField();

    Direction GetRightDirection(Direction dir) const;
private:
    Field field;
    Snake snake;
    Fruit fruit;

    Direction currentDirection = Direction::Up;

    SnakeController* controller = nullptr;

    std::uint32_t score{ 0 };
    std::int32_t movesLeft{ 50 };

    float frameTime = 0.f;
    bool isInProgress = false;
};