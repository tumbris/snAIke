#include <snAIke/SnakeGame/SnakeGame.hpp>

#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Application/Application.hpp>
#include <snAIke/ImGui/ImGuiRenderWindow.hpp>
#include <snAIke/Utility/Random.hpp>
#include <snAIke/Utility/CoreDefs.hpp>

#include <snAIke/ImGui/ImGuiSnakeController.hpp>                                    //DefaultConstroller

#include <SFML/Graphics.hpp>

#include <imgui.h>

SnakeGame::~SnakeGame()
{
    if (auto director = Singleton<Director>::GetInstance())
    {
        for (auto id : callbacks)
        {
            director->RemoveUpdatorCallback(id);
        }
    }
}

void SnakeGame::Init(std::size_t fieldSize, const IntrusivePtr<SnakeController>& controller)
{
    field.Init(fieldSize);
    this->controller = controller;

    if (auto director = Singleton<Director>::GetInstance())
    {
        callbacks.push_back(director->AddUpdatorCallback(Callback(this, &SnakeGame::Update), UpdatePriority::GameUpdate));
        callbacks.push_back(director->AddUpdatorCallback(Callback(this, &SnakeGame::ImGuiRender), UpdatePriority::ImGuiUpdate));
        callbacks.push_back(director->AddUpdatorCallback(Callback(this, &SnakeGame::DebugDraw)));

        renderTarget = director->GetMainApplication()->GetGameRenderWindow();
    }
    SpawnSnake();
    SpawnFruit();
}

void SnakeGame::SetController(const IntrusivePtr<SnakeController>& controller)
{
    this->controller = controller;
}

void SnakeGame::Update(float dt)
{
    auto targetSize = renderTarget->GetSize();

    auto newFieldSize = targetSize * 0.95f;
    field.ScaleToFit(newFieldSize.x, newFieldSize.y);
    newFieldSize = field.GetSize();

    ImVec2 desiredPos = (targetSize - newFieldSize) / 2;

    field.setPosition(desiredPos.x, desiredPos.y);

    if (isInProgress)
    {
        frameTime += dt;
        while (frameTime > TIME_PER_TICK)
        {
            Tick();
            frameTime -= TIME_PER_TICK;
        }
        frameTime = std::max(0.f, frameTime);
    }
}

bool SnakeGame::IsInProgress() const
{
    return isInProgress;
}

void SnakeGame::StartGame()
{
    isInProgress = true;
    score = 0;

    if (!controller)
    {
        controller = MakeIntrusive<ImGuiSnakeController>();
    }
}

void SnakeGame::SpawnSnake()
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

void SnakeGame::SpawnFruit()
{
    if (fruit.x != -1 || fruit.y != -1) return;

    auto fieldSize = field.GetFieldSize();

    if (snake.size() == static_cast<std::size_t>(fieldSize * fieldSize)) return;

    Fruit f
    {
        RandInRange(fieldSize / 2, fieldSize - 1),
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

void SnakeGame::ImGuiRender(float)
{
    renderTarget->Draw(field);

    ImGui::Begin("Snake Game");
    {
        auto windowSize = ImGui::GetWindowContentRegionWidth();
        const auto buttonWidth = windowSize / 1.5f;

        ImGui::Text("Score: %d", score);

        if (!isInProgress)
        {
            ImGui::Indent((windowSize / 2.f) - (buttonWidth / 2.f));
            if (ImGui::Button("Start", { buttonWidth, 0.f }))
            {
                StartGame();
            }
            ImGui::Unindent((windowSize / 2.f) - (buttonWidth / 2.f));
        }
    }
    ImGui::End();
}

void SnakeGame::Tick()
{
    if (controller)
    {
        Direction dir = controller->GetDirection(field.GetTileGrid());
        dir = GetRightDirection(dir);
        currentDirection = dir;

        if (snake[0].x == fruit.x && snake[0].y == fruit.y)
        {
            ++score;
            snake.push_back(snake[0]);
            fruit = { -1, -1 };
            SpawnFruit();
        }

        UpdateSnakePosition(dir);
        CheckBounds();

        for (std::size_t i = 1; i < snake.size(); ++i)
        {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                isInProgress = false;
                currentDirection = Direction::Up;

                SpawnSnake();
                SpawnFruit();
                break;
            }
        }

        RefreshField();
    }
}

void SnakeGame::UpdateSnakePosition(Direction dir)
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

void SnakeGame::CheckBounds()
{
    auto fieldSize = field.GetFieldSize();

    if (snake[0].x >= fieldSize)
    {
        snake[0].x = 0;
    }
    if (snake[0].x < 0)
    {
        snake[0].x = fieldSize - 1;;
    }
    if (snake[0].y >= fieldSize)
    {
        snake[0].y = 0;
    }
    if (snake[0].y < 0)
    {
        snake[0].y = fieldSize - 1;;
    }
}

void SnakeGame::RefreshField()
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

Direction SnakeGame::GetRightDirection(Direction dir) const
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


#if defined(DEBUG)
void SnakeGame::DebugDraw(float)
{
    //sf::RenderWindow* mainWindow = Singleton<Director>::GetInstance()->GetMainApplication()->GetApplicationRenderWindow();
    //mainWindow->draw(field);
}
#endif