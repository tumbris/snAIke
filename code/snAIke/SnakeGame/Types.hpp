#pragma once

#include <vector>

enum TileType
{
    TileType_Empty,
    TileType_SnakeBody,
    TileType_SnakeHead,
    TileType_Fruit,
    Count
};

using TileGrid = std::vector<std::vector<TileType>>;

struct SnakeSegment
{
    std::int64_t x{ 0 };
    std::int64_t y{ 0 };
};

struct Fruit
{
    std::int64_t x{ -1 };
    std::int64_t y{ -1 };
};

using Snake = std::vector<SnakeSegment>;

enum class Direction
{
    Up = 0,
    Right = 1,
    Left = 2,
    Down = 3
};