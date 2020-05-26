#include <snAIke/Singletons/Director/Director.hpp>

#include <snAIke/Application/Application.hpp>

#include <SFML/Graphics.hpp>
#include "Director.hpp"

UpdatorIdType Director::idProvider = { 0 };

Director::Director()
{
    mainApp = new Application();
}

Director::~Director()
{
    delete mainApp;
}

void Director::InitMainApplication()
{
    const sf::VideoMode& videoMode = sf::VideoMode::getDesktopMode();
    mainApp->Init(videoMode.width, videoMode.height, "snAIke");
}

Application* Director::GetMainApplication()
{
    return mainApp;
}

void Director::RemoveUpdatorCallback(UpdatorIdType id)
{
    for (auto& [_, updatorsMap] : updators)
    {
        auto it = updatorsMap.find(id);
        if (it != updatorsMap.end())
        {
            updatorsMap.erase(it);
            return;
        }
    }
}

void Director::Emplace(Updator&& updator, UpdatePriority priority)
{
    updators[priority].emplace(updator.GetId(), std::move(updator));
}

void Director::TriggerUpdates(UpdatePriority priority)
{
    auto dt = mainApp->GetFrameTime();
    for (auto& [id, updator] : updators[priority])
    {
        updator.Call(dt);
    }
}
