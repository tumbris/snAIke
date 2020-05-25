#include <snAIke/Singletons/Director.hpp>

#include <snAIke/Application/Application.hpp>

Director::Director()
{
    mainApp = new Application();
}

Director::~Director()
{
    delete mainApp;
}

Application* Director::GetMainApplication()
{
    return mainApp;
}
