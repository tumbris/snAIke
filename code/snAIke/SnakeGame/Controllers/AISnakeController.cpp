#include <snAIke/SnakeGame/Controllers/AISnakeController.hpp>

#include <snAIke/Singletons/GameManager.hpp>
#include <snAIke/Utility/Callback.hpp>
#include <algorithm>
#include <bitset>

#include <imgui.h>

#if defined(DEBUG)
#include <iostream>
#include <iomanip>
#endif

void AISnakeController::Init(const IntrusivePtr<FeedForwardNN>& network)
{
    this->network = network;
}

Direction AISnakeController::GetDirection()
{

    auto inputs = CalculateInputs();
    auto outputs = network->Run(inputs);

#if defined(DEBUG)
    std::cout << "Outputs:\t";
    for (auto output : outputs) std::cout << std::left << std::setw(9) << output << ' ';
    std::cout << '\n';
#endif

    auto max_output = std::max_element(outputs.begin(), outputs.end());
    auto index = std::distance(outputs.begin(), max_output);
    switch (index)
    {
    case 0: return Direction::Up;
    case 1: return Direction::Right;
    case 2: return Direction::Left;
    case 3: return Direction::Down;
    }
    return Direction::Up;
}

void AISnakeController::ImGuiUpdate_Impl()
{
}

std::vector<float> AISnakeController::CalculateInputs() const
{
    std::vector<float> inputs(20);
    float fieldSize = static_cast<float>(GameManager::fieldSize);
    inputs[0] = static_cast<float>(GetFoodDistanse(Up)) / fieldSize * 10.f - 5.f;
    inputs[1] = static_cast<float>(GetFoodDistanse(Right)) / fieldSize * 10.f - 5.f;
    inputs[2] = static_cast<float>(GetFoodDistanse(Down)) / fieldSize * 10.f - 5.f;
    inputs[3] = static_cast<float>(GetFoodDistanse(Left)) / fieldSize * 10.f - 5.f;
    inputs[4] = static_cast<float>(GetFoodDistanse(UpRight)) / fieldSize * 10.f - 5.f;
    inputs[5] = static_cast<float>(GetFoodDistanse(UpLeft)) / fieldSize * 10.f - 5.f;
    inputs[6] = static_cast<float>(GetFoodDistanse(DownRight)) / fieldSize * 10.f - 5.f;
    inputs[7] = static_cast<float>(GetFoodDistanse(DownLeft)) / fieldSize * 10.f - 5.f;

    inputs[8] = static_cast<float>(GetTailDistanse(Up)) / fieldSize * 10.f - 5.f;
    inputs[9] = static_cast<float>(GetTailDistanse(Right)) / fieldSize * 10.f - 5.f;
    inputs[10] = static_cast<float>(GetTailDistanse(Down)) / fieldSize * 10.f - 5.f;
    inputs[11] = static_cast<float>(GetTailDistanse(Left)) / fieldSize * 10.f - 5.f;
    inputs[12] = static_cast<float>(GetTailDistanse(UpRight)) / fieldSize * 10.f - 5.f;
    inputs[13] = static_cast<float>(GetTailDistanse(UpLeft)) / fieldSize * 10.f - 5.f;
    inputs[14] = static_cast<float>(GetTailDistanse(DownRight)) / fieldSize * 10.f - 5.f;
    inputs[15] = static_cast<float>(GetTailDistanse(DownLeft)) / fieldSize * 10.f - 5.f;

    inputs[16] = static_cast<float>(GetWallDistanse(Up)) / fieldSize * 10.f - 5.f;
    inputs[17] = static_cast<float>(GetWallDistanse(Right)) / fieldSize * 10.f - 5.f;
    inputs[18] = static_cast<float>(GetWallDistanse(Down)) / fieldSize * 10.f - 5.f;
    inputs[19] = static_cast<float>(GetWallDistanse(Left)) / fieldSize * 10.f - 5.f;


    return inputs;
}

std::size_t AISnakeController::GetFoodDistanse(LookDirection dir) const
{
    const auto& snake = Singleton<GameManager>::GetInstance()->GetSnake();
    const auto& fruit = Singleton<GameManager>::GetInstance()->GetFruit();

    if (dir != Up && dir != Right && dir != Left && dir != Down)
    {
        if (std::abs(snake[0].x - fruit.x) == std::abs(snake[0].y - fruit.y))
        {
            if (dir & Down && fruit.x > snake[0].x)
            {
                if (dir & Left && fruit.y < snake[0].y) return static_cast<std::size_t>(snake[0].y - fruit.y);
                if (dir & Right && fruit.y > snake[0].y) return static_cast<std::size_t>(fruit.y - snake[0].y);
            }
            if (dir & Up && fruit.x < snake[0].x)
            {
                if (dir & Left && fruit.y < snake[0].y) return static_cast<std::size_t>(snake[0].y - fruit.y);
                if (dir & Right && fruit.y > snake[0].y) return static_cast<std::size_t>(fruit.y - snake[0].y);
            }
        }
    }
    else
    {
        switch (dir)
        {
        case Up:
            if (fruit.y == snake[0].y && fruit.x < snake[0].x) return static_cast<std::size_t>(snake[0].x - fruit.x);
            break;
        case Right:
            if (fruit.x == snake[0].x && fruit.y > snake[0].y) return static_cast<std::size_t>(fruit.y - snake[0].y);
            break;
        case Down:
            if (fruit.y == snake[0].y && fruit.x > snake[0].x) return static_cast<std::size_t>(fruit.x - snake[0].x);
            break;
        case Left:
            if (fruit.x == snake[0].x && fruit.y < snake[0].y) return static_cast<std::size_t>(snake[0].y - fruit.y);
            break;
        }
    }
    return GameManager::fieldSize;
}

std::size_t AISnakeController::GetTailDistanse(LookDirection dir) const
{
    const auto& snake = Singleton<GameManager>::GetInstance()->GetSnake();
    if (dir == Up || dir == Down)
    {
        for (std::size_t i = 0; i < snake.size(); ++i)
        {
            if (snake[0].y == snake[i].y)
            {
                if (dir == Up && snake[0].x > snake[i].x) return static_cast<std::size_t>(snake[0].x - snake[i].x);
                if (dir == Down && snake[0].x < snake[i].x) return static_cast<std::size_t>(snake[i].x - snake[0].x);
            }
        }
    }
    else if (dir == Left || dir == Right)
    {
        for (std::size_t i = 0; i < snake.size(); ++i)
        {
            if (snake[0].x == snake[i].x)
            {
                if (dir == Left && snake[0].y > snake[i].y) return static_cast<std::size_t>(snake[0].y - snake[i].y);
                if (dir == Right && snake[0].y < snake[i].y) return static_cast<std::size_t>(snake[i].y - snake[0].y);
            }
        }
    }
    else
    {
        for (std::size_t i = 0; i < snake.size(); ++i)
        {
            if (std::abs(snake[0].x - snake[i].x) == std::abs(snake[0].y - snake[i].y))
            {
                if (dir & Down && snake[i].x > snake[0].x)
                {
                    if (dir & Left && snake[i].y < snake[0].y) return static_cast<std::size_t>(snake[0].y - snake[i].y);
                    if (dir & Right && snake[i].y > snake[0].y) return static_cast<std::size_t>(snake[i].y - snake[0].y);
                }
                if (dir & Up && snake[i].x < snake[0].x)
                {
                    if (dir & Left && snake[i].y < snake[0].y) return static_cast<std::size_t>(snake[0].y - snake[i].y);
                    if (dir & Right && snake[i].y > snake[0].y) return static_cast<std::size_t>(snake[i].y - snake[0].y);
                }
            }
        }
    }
    return GameManager::fieldSize;
}

std::size_t AISnakeController::GetWallDistanse(LookDirection dir) const
{
    const auto& snake = Singleton<GameManager>::GetInstance()->GetSnake();
    auto fieldSize = GameManager::fieldSize;

    if (dir == Up) return snake[0].x;
    if (dir == Down) return fieldSize - snake[0].x - 1;
    if (dir == Right) return fieldSize - snake[0].y - 1;
    if (dir == Left) return snake[0].y;
    return fieldSize;
}
