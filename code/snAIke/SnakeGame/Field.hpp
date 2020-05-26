#pragma once

#include <snAIke/SnakeGame/Types.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>
#include <array>

class Field : public sf::Drawable, public sf::Transformable
{
public:
    Field();
    
    void Init(std::size_t fieldSize);                                                       //Field wil be fieldSize X fieldSize

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void ScaleToFit(float x, float y);
    sf::Vector2u GetSize() const;

    std::int64_t GetFieldSize() const;

    const TileGrid& GetTileGrid() const;
    void SetTileGrid(const TileGrid& grid);
    void SetTileGrid(TileGrid&& grid);

    void SetTile(std::size_t i, std::size_t j, TileType tile);

private:
    std::array<sf::Sprite, TileType::Count> tileTemplates;
    TileGrid tiles;

    std::size_t fieldSize = 0;
    std::size_t tileSize = 0;

private:
    void LoadTextures();
};

