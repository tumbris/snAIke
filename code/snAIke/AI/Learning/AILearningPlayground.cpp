#include <snAIke/AI/Learning/AILearningPlayground.hpp>

#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Utility/Random.hpp>

#include <snAIke/SnakeGame/Controllers/AISnakeController.hpp>

#include <snAIke/Singletons/AIManager.hpp>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui_internal.h>

void AILearningPlayground::Init(std::size_t fieldSize)
{
    field.Init(fieldSize);
 
    SpawnSnake();
    SpawnFruit();

    RefreshField();
}

AISnakeData AILearningPlayground::Run(const std::vector<float>& weights, const std::vector<float>& biases)
{
    auto ffnn = MakeIntrusive<FeedForwardNN>();

    const auto& layer_config = Singleton<AIManager>::GetInstance()->GetLayersConfig();

    ffnn->Init(layer_config);

    ffnn->SetWeights(AIManager::ConvertToWeights(weights, layer_config));
    ffnn->SetBiases(AIManager::ConvertToBiases(biases, layer_config));

    auto ai_controller = new AISnakeController();
    ai_controller->Init(ffnn);
    controller = ai_controller;

    StartGame();
    std::int64_t lifetime = 0;
    while (isInProgress)
    {
        Tick();
        --movesLeft;
        ++lifetime;
    }

    delete ai_controller;
    return { score, lifetime };

}

void AILearningPlayground::StartGame()
{
    isInProgress = true;
    score = 0;
    {
        controller->Bind(field.GetTileGrid());
        controller->Bind(snake);
    }
}

void AILearningPlayground::StopGame()
{
    isInProgress = false;
    currentDirection = Direction::Up;

    auto fieldSize = field.GetFieldSize();

    SnakeSegment snakeStart =
    {
        fieldSize / 2,
        fieldSize / 2
    };

    snake.clear();
    snake.reserve(snakeInitSize);

    SpawnSnake();
    SpawnFruit();

    RefreshField();
}

void AILearningPlayground::SpawnSnake()
{
    static_assert(snakeInitSize >= 2, "Snake should be longer than two!");

    snake.clear();
    snake.reserve(snakeInitSize);

    auto fieldSize = field.GetFieldSize();

    SnakeSegment snakeStart =
    {
        RandInRange(fieldSize / 2, fieldSize - 1),
        RandInRange(0ll, fieldSize - 1)
    };

    for (std::size_t i = 0; i < snakeInitSize; ++i)
    {
        snake.push_back(
            {
                static_cast<std::int64_t>(snakeStart.x - (snakeInitSize - i - 1)),
                static_cast<std::int64_t>(snakeStart.y)
            });
    }


    field.SetTile(snake[0].y, snake[0].x, TileType_SnakeHead);

    for (std::size_t i = 1; i < snake.size(); ++i)
    {
        field.SetTile(snake[i].y, snake[i].x, TileType_SnakeBody);
    }
}

void AILearningPlayground::SpawnFruit()
{
    auto fieldSize = field.GetFieldSize();

    if (snake.size() == static_cast<std::size_t>(fieldSize * fieldSize)) return;

    Fruit f
    {
        RandInRange(0ll, fieldSize - 1),
        RandInRange(0ll, fieldSize - 1)
    };

    auto isFruiteInSnake = [&]()
    {
        for (auto s : snake)
        {
            if (s.x == f.x && s.y == f.y)
            {
                return true;
            }
        }
        return false;
    };

    while (isFruiteInSnake())
    {
        f.x = RandInRange(fieldSize / 2, fieldSize - 1);
        f.y = RandInRange(0ll, fieldSize - 1);
    }

    fruit = f;

    field.SetTile(static_cast<std::size_t>(fruit.y), static_cast<std::size_t>(fruit.x), TileType_Fruit);
}

void AILearningPlayground::ImGuiRender(float dt)
{
    
}

void AILearningPlayground::Tick()
{
    if (controller)
    {
        Direction dir = controller->GetDirection();
        dir = GetRightDirection(dir);
        currentDirection = dir;

        if (snake[0].x == fruit.x && snake[0].y == fruit.y)
        {
            ++score;
            movesLeft += 50;
            snake.push_back(snake[0]);
            SpawnFruit();
        }

        UpdateSnakePosition(dir);

        if (CheckBounds() || movesLeft <= 0)
        {
            StopGame();
            return;
        }

        for (std::size_t i = 1; i < snake.size(); ++i)
        {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                StopGame();
                break;
            }
        }

        RefreshField();
    }
}

void AILearningPlayground::UpdateSnakePosition(Direction dir)
{

    for (std::size_t i = snake.size() - 1; i > 0; --i)
    {
        snake[i] = snake[i - 1];
    }

    switch (dir)
    {
    case Direction::Up: --snake[0].x; break;
    case Direction::Down: ++snake[0].x; break;
    case Direction::Right: ++snake[0].y; break;
    case Direction::Left: --snake[0].y; break;
    }
}

bool AILearningPlayground::CheckBounds()
{
    auto fieldSize = field.GetFieldSize();

    if (snake[0].x >= fieldSize) return true;
    if (snake[0].x < 0) return true;
    if (snake[0].y >= fieldSize) return true;
    if (snake[0].y < 0) return true;

    return false;
}

void AILearningPlayground::RefreshField()
{
    const auto& tiles = field.GetTileGrid();
    for (std::size_t i = 0; i < tiles.size(); ++i)
    {
        for (std::size_t j = 0; j < tiles[i].size(); ++j)
        {
            field.SetTile(i, j, TileType_Empty);
        }
    }

    if (fruit.x >= 0 && fruit.y >= 0)
    {
        field.SetTile(static_cast<std::size_t>(fruit.y), static_cast<std::size_t>(fruit.x), TileType_Fruit);
    }

    field.SetTile(static_cast<std::size_t>(snake[0].y), static_cast<std::size_t>(snake[0].x), TileType_SnakeHead);

    for (std::size_t i = 1; i < snake.size(); ++i)
    {
        field.SetTile(static_cast<std::size_t>(snake[i].y), static_cast<std::size_t>(snake[i].x), TileType_SnakeBody);
    }
}

Direction AILearningPlayground::GetRightDirection(Direction dir) const
{
    switch (currentDirection)
    {
    case Direction::Up: return dir != Direction::Down ? dir : currentDirection;
    case Direction::Down: return dir != Direction::Up ? dir : currentDirection;
    case Direction::Left: return dir != Direction::Right ? dir : currentDirection;
    case Direction::Right: return dir != Direction::Left ? dir : currentDirection;
    default: return dir;
    }

    return dir;
}
