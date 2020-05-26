#include <snAIke/Singletons/Singleton.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <string>
#include <map>

class ResourceManager
{
private:
    ResourceManager() = default;
    ~ResourceManager();

public:
    sf::Texture& GetTextureResource(const std::string& filepath);

private:
    std::map<std::string, sf::Texture> textureResources;

private:
    sf::Texture& LoadTextureResource(const std::string& filepath);
    void UnloadTextureResources();

    friend class Singleton<ResourceManager>;
};