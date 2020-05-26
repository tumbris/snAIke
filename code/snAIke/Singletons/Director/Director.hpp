#pragma once

#include <snAIke/Singletons/Director/Updator.hpp>
#include <snAIke/Singletons/Singleton.hpp>
#include <snAIke/Utility/UpdatePriority.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <vector>
#include <functional>
#include <map>

class Application;

class Director
{
private:
    Director();
    ~Director();
public:
    
    void InitMainApplication();

    Application* GetMainApplication();

    template <class Fn>
    UpdatorIdType AddUpdatorCallback(Fn&& updator, UpdatePriority priority = UpdatePriority::Default);
    void RemoveUpdatorCallback(UpdatorIdType id);

private:
    void Emplace(Updator&& updator, UpdatePriority priority);

    void TriggerUpdates(UpdatePriority priority);
private:
    std::map<UpdatePriority, std::map<UpdatorIdType, Updator>> updators;
    Application* mainApp = nullptr;

    static UpdatorIdType idProvider;

private:
    friend class Singleton<Director>;
    friend class Application;           //To triger proper updates
};

template<class Fn>
UpdatorIdType Director::AddUpdatorCallback(Fn&& updator, UpdatePriority priority)
{
    UpdatorIdType id = idProvider++;
    Emplace(Updator(std::forward<Fn>(updator), id), priority);
    return id;
}
