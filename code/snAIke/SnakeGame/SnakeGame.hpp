#pragma once

#include <snAIke/SnakeGame/Field.hpp>
#include <snAIke/Singletons/Director/Types.hpp>
#include <snAIke/Utility/Ref/IntrusivePtr.hpp>
#include <snAIke/SnakeGame/Controllers/SnakeController.hpp>

#include <SFML\include\SFML\Window\Keyboard.hpp>

#include <functional>
#include <vector>

class ImGuiRenderWindow;

class SnakeGame
{
private:
    static constexpr std::size_t snakeInitSize = 4;
public:
    ~SnakeGame();

    void Init(std::size_t fieldSize);

    void PushController(const IntrusivePtr<SnakeController>& controller);

    void Update(float dt);

    bool IsInProgress() const;

    const Snake& GetSnake() const { return snake; };
    const Fruit& GetFruit() const { return fruit; };

    template <class Fn>
    void SetOnGameEndCallback(Fn&& callback);

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

    std::function<void(SnakeGame*)> onGameEndCallback;
    std::vector<UpdatorIdType> callbacks;

    std::vector<IntrusivePtr<SnakeController>> controllers;
    SnakeController* pickedController = nullptr;

    ImGuiRenderWindow* renderTarget;

    std::uint32_t score{0};

    float frameTime = 0.f;
    bool isInProgress = false;

#if defined(DEBUG)
    void DebugDraw(float);
#endif
};


template<class Fn>
void SnakeGame::SetOnGameEndCallback(Fn&& callback)
{
    onGameEndCallback = std::forward<Fn>(callback);
}
