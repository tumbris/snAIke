#pragma once

#include <snAIke/Singletons/Singleton.hpp>

class Application;

class Director
{
private:
    Director();
    ~Director();
public:
    
    Application* GetMainApplication();

private:
    Application* mainApp = nullptr;
private:
    friend class Singleton<Director>;
};