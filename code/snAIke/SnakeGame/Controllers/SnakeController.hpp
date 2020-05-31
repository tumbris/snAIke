#pragma once

#include <snAIke/SnakeGame/Types.hpp>
#include <snAIke/Singletons/Director/Types.hpp>
#include <snAIke/Utility/Ref/ReferenceCountable.hpp>

class SnakeController : public ReferenceCountable<DefaultThreadPolicy>
{
public:
    SnakeController();
    virtual ~SnakeController();

    virtual Direction GetDirection() = 0;
    virtual const char* GetName() const = 0;
    
    void Bind(const TileGrid& grid);
    void Bind(const Snake& snake);
    void Update(float dt);
    void ImGuiUpdate(float);

protected:
    virtual void Update_Impl(float) {}
    virtual void ImGuiUpdate_Impl() {}

    const Snake& GetSnake() const;
    Fruit GetFruit() const;

protected:
    const TileGrid* field = nullptr;
    const Snake* snake = nullptr;
    std::vector<UpdatorIdType> callbacks;
};