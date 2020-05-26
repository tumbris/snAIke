#include <snAIke/Singletons/ResourceManager.hpp>

ResourceManager::~ResourceManager()
{
    UnloadTextureResources();
}

sf::Texture& ResourceManager::GetTextureResource(const std::string& filepath)
{
    auto textureResource = textureResources.find(filepath);
    if (textureResource != textureResources.end())
    {
        return textureResource->second;
    }
    return LoadTextureResource(filepath);
}

sf::Texture& ResourceManager::LoadTextureResource(const std::string& filepath)
{
    static const std::string res_path = RES_PATH"/";

    auto& texture = textureResources[filepath];
    texture.loadFromFile(res_path + filepath + RES_EXTENTION);
    return texture;
}

void ResourceManager::UnloadTextureResources()
{
    textureResources.clear();
}
