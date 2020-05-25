#include <snAIke/Singletons/Singletons.hpp>

#include <snAIke/Singletons/ApplicationArgs.hpp>
#include <snAIke/Singletons/Director.hpp>

void InitializeSingletons()
{
    Singleton<ApplicationArgs>::CreateInstance();
    Singleton<Director>::CreateInstance();
}

void DeinitializeSingletons()
{
    Singleton<ApplicationArgs>::DestroyInstance();
    Singleton<Director>::DestroyInstance();
}
