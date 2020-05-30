#include <snAIke/Singletons/GameManager.hpp>
#include <snAIke/SnakeGame/SnakeGame.hpp>

#include <snAIke/SnakeGame/Controllers/ImGuiSnakeController.hpp>

GameManager::GameManager()
    : game(std::make_unique<SnakeGame>())
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
    game->Init(fieldSize);
    
    //default controller
    auto imguiController = MakeIntrusive<ImGuiSnakeController>();
    PushController(imguiController);
}

void GameManager::PushController(const IntrusivePtr<SnakeController>& controller)
{
    game->PushController(controller);
}

const Snake& GameManager::GetSnake() const
{
    return game->GetSnake();
}

const Fruit& GameManager::GetFruit() const
{
    return game->GetFruit();
}
