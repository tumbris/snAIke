#pragma once

#include <snAIke/Utility/Ref/IntrusivePtr.hpp>
#include <snAIke/Singletons/Singleton.hpp>

#include <snAIke/SnakeGame/Types.hpp>

#include <memory>

class SnakeGame;
class SnakeController;

class GameManager
{
public:
    static constexpr std::size_t fieldSize = 32;
private:
    GameManager();
    ~GameManager();

public:
    void Init();
    void PushController(const IntrusivePtr<SnakeController>& controller);

    const Snake& GetSnake() const;
    const Fruit& GetFruit() const;

private:
    std::unique_ptr<SnakeGame> game;
private:
    friend class Singleton<GameManager>;
};