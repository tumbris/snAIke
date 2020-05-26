#include <snAIke/Application/Application.hpp>
#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Singletons/Singletons.hpp>

char** g_argv = nullptr;

int main(int argc, char** argv)
{
    g_argv = argv;
    InitializeSingletons();
    
    int res = EXIT_FAILURE;

    if (Director* director = Singleton<Director>::GetInstance())
    {
        director->InitMainApplication();
        if (Application* app = director->GetMainApplication())
        {
            res = app->Run();
        }
    }

    DeinitializeSingletons();
    return res;
}