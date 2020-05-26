#pragma once

#include <snAIke/SnakeGame/Types.hpp>
#include <snAIke/Utility/Ref/ReferenceCountable.hpp>

class SnakeController : public ReferenceCountable<DefaultThreadPolicy>
{
public:
    virtual ~SnakeController() = default;

    virtual Direction GetDirection(const TileGrid& field) = 0;
};