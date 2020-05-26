#include <snAIke/Singletons/Singletons.hpp>

#include <snAIke/Singletons/ApplicationArgs.hpp>
#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Singletons/ResourceManager.hpp>

void InitializeSingletons()
{
    Singleton<ApplicationArgs>::CreateInstance();
    Singleton<ResourceManager>::CreateInstance();
    Singleton<Director>::CreateInstance();
}

void DeinitializeSingletons()
{
    //In reverse order

    Singleton<Director>::DestroyInstance();
    Singleton<ResourceManager>::DestroyInstance();
    Singleton<ApplicationArgs>::DestroyInstance();
}
