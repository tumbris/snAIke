#include <snAIke/Singletons/AIManager.hpp>

#include <snAIke/SnakeGame/Controllers/AISnakeController.hpp>
#include <snAIke/Singletons/GameManager.hpp>

#include <algorithm>

AIManager::AIManager()
{
    aiController = MakeIntrusive<AISnakeController>();

    const auto fieldSize = GameManager::fieldSize;

    static FeedForwardNN::Config config;
    config.layers_sizes =
    {
        20,
        18,
        18
    };
    config.outputs_count = 4;
    layerConfig = config;

    network = MakeIntrusive<FeedForwardNN>();

    if (!network->LoadFromFile())
    {
        network->Init(layerConfig);
        network->SetRandomWeights();
    }

    aiController->Init(network);

    if (auto gm = Singleton<GameManager>::GetInstance())
    {
        gm->PushController(aiController);
    }
}

AIManager::~AIManager()
{
    network->SaveToFile();
}
