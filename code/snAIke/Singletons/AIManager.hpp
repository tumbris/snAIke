#pragma once

#include <snAIke/Singletons/Singleton.hpp>
#include <snAIke/AI/NeuralNetwork/FeedForwardNN.hpp>

class AISnakeController;

#include <snAIke/Utility/Ref/IntrusivePtr.hpp>

class AIManager
{
private:
    AIManager();
    ~AIManager();

private:
    FeedForwardNN::Config layerConfig;
    IntrusivePtr<FeedForwardNN> network;
    IntrusivePtr<AISnakeController> aiController;
private:
    friend class Singleton<AIManager>;
};