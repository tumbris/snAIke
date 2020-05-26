#pragma once

#include <snAIke/SnakeGame/Field.hpp>
#include <snAIke/Singletons/Director/Types.hpp>
#include <snAIke/Utility/Ref/IntrusivePtr.hpp>
#include <snAIke/SnakeGame/SnakeController.hpp>

#include <vector>
#include <SFML\include\SFML\Window\Keyboard.hpp>

class ImGuiRenderWindow;

class SnakeGame
{
private:
    static constexpr std::size_t snakeInitSize = 4;
public:
    ~SnakeGame();

    void Init(std::size_t fieldSize, const IntrusivePtr<SnakeController>& controller = nullptr);

    void SetController(const IntrusivePtr<SnakeController>& controller);

    void Update(float dt);

    bool IsInProgress() const;

    void StartGame();

private:
    void SpawnSnake();
    void SpawnFruit();
    void ImGuiRender(float);

    void Tick();

    void UpdateSnakePosition(Direction dir);
    void CheckBounds();
    void RefreshField();

    Direction GetRightDirection(Direction dir) const;
private:

    Field field;
    Snake snake;
    Fruit fruit;

    Direction currentDirection = Direction::Up;

    std::vector<UpdatorIdType> callbacks;

    IntrusivePtr<SnakeController> controller;
    ImGuiRenderWindow* renderTarget;

    std::uint32_t score{0};

    float frameTime = 0.f;
    bool isInProgress = false;

#if defined(DEBUG)
    void DebugDraw(float);
#endif
};
