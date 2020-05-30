#include <snAIke/Singletons/Singletons.hpp>

#include <snAIke/Singletons/ApplicationArgs.hpp>
#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Singletons/ResourceManager.hpp>
#include <snAIke/Singletons/GameManager.hpp>
#include <snAIke/Singletons/AIManager.hpp>

void InitializeSingletons()
{
    Singleton<ApplicationArgs>::CreateInstance();
    Singleton<ResourceManager>::CreateInstance();
    Singleton<Director>::CreateInstance();
    Singleton<GameManager>::CreateInstance();
    Singleton<AIManager>::CreateInstance();
}

void DeinitializeSingletons()
{
    //In reverse order
    Singleton<AIManager>::DestroyInstance();
    Singleton<GameManager>::DestroyInstance();
    Singleton<Director>::DestroyInstance();
    Singleton<ResourceManager>::DestroyInstance();
    Singleton<ApplicationArgs>::DestroyInstance();
}
