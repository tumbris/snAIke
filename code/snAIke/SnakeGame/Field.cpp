#include <snAIke/SnakeGame/Field.hpp>
#include <snAIke/Singletons/ResourceManager.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>

Field::Field()
{
}

void Field::Init(std::size_t fieldSize)
{
    LoadTextures();
    this->fieldSize = fieldSize;
    tiles.resize(fieldSize);

    for (auto& row : tiles)
    {
        row.resize(fieldSize);
        std::fill(row.begin(), row.end(), TileType_Empty);
    }
}

void Field::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (std::size_t i = 0; i < tiles.size(); ++i)
    {
        for (std::size_t j = 0; j < tiles[i].size(); ++j)
        {
            sf::Sprite tile = tileTemplates[tiles[i][j]];
            float tileSize = static_cast<float>(tile.getTextureRect().width);

            tile.setPosition(i * tileSize, j * tileSize);
            target.draw(tile, states);
        }
    }
}

void Field::ScaleToFit(float x, float y)
{
    float curX = static_cast<float>(fieldSize * tileSize);
    float curY = static_cast<float>(fieldSize * tileSize);

    float ratio = std::min(x / curX, y / curY);
    setScale(ratio, ratio);
}

sf::Vector2u Field::GetSize() const
{
    float x = static_cast<float>(fieldSize * tileSize);
    float y = x;
    auto scale = getScale();
    x = x * scale.x;
    y = y * scale.y;
    return { static_cast<unsigned int>(x), static_cast<unsigned int>(y) };
}

std::int64_t Field::GetFieldSize() const
{
    return static_cast<std::int64_t>(fieldSize);
}

const TileGrid& Field::GetTileGrid() const
{
    return tiles;
}

void Field::SetTileGrid(const TileGrid& grid)
{
    tiles = grid;
}

void Field::SetTileGrid(TileGrid&& grid)
{
    tiles.operator=(std::move(grid));
}

void Field::SetTile(std::size_t i, std::size_t j, TileType tile)
{
    tiles[i][j] = tile;
}

void Field::LoadTextures()
{
    auto resourceManager = Singleton<ResourceManager>::GetInstance();

    tileTemplates[TileType_Empty].setTexture(resourceManager->GetTextureResource("Empty"));
    tileTemplates[TileType_SnakeBody].setTexture(resourceManager->GetTextureResource("SnakeBody"));
    tileTemplates[TileType_SnakeHead].setTexture(resourceManager->GetTextureResource("SnakeHead"));
    tileTemplates[TileType_Fruit].setTexture(resourceManager->GetTextureResource("Fruit"));

    tileSize = tileTemplates.front().getTexture()->getSize().x;

#ifndef NDEBUG
    const auto size = tileTemplates.front().getTexture()->getSize();
    for (const auto& sprite : tileTemplates)
    {
        assert(size == sprite.getTexture()->getSize());                                 //tiles should be equal in size
        assert(sprite.getTexture()->getSize().x == sprite.getTexture()->getSize().y);   //tiles should be square
    }
#endif
}