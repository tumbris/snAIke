#include <snAIke/Singletons/AIManager.hpp>

#include <snAIke/SnakeGame/Controllers/AISnakeController.hpp>
#include <snAIke/Singletons/GameManager.hpp>
#include <snAIke/AI/Learning/AILearningPlayground.hpp>
#include <snAIke/AI/Learning/GeneticAlgorithm.hpp>

#include <snAIke/Singletons/Director/Director.hpp>
#include <snAIke/Utility/Callback.hpp>

#include <imgui.h>

#include <algorithm>

AIManager::AIManager()
{
    aiController = MakeIntrusive<AISnakeController>();

    const auto fieldSize = GameManager::fieldSize;

    NNLayersConfig config;
    config.layers_sizes =
    {
        24,
        18,
        18,
        4
    };
    layerConfig = config;

    network = MakeIntrusive<FeedForwardNN>();

    if (!network->LoadFromFile())
    {
        network->Init(layerConfig);
        network->Randomize();
    }

    ga.GenerateInitialPopulation(ConvertToVectorF(network->GetWeights()), ConvertToVectorF(network->GetBiases()));
    aiController->Init(network);

    if (auto gm = Singleton<GameManager>::GetInstance())
    {
        gm->PushController(aiController);
    }

    if (auto director = Singleton<Director>::GetInstance())
    {
        imGuiUpdatorCallbackId = director->AddUpdatorCallback(Callback(this, &AIManager::ImGuiUpdate), UpdatePriority::ImGuiUpdate);
    }
}

AIManager::~AIManager()
{
    network->SaveToFile();

    if (auto director = Singleton<Director>::GetInstance())
    {
        director->RemoveUpdatorCallback(imGuiUpdatorCallbackId);
    }
}


AISnakeData AIManager::GenerateDataForSnakeThreaded(const AISnake& snake)
{
    AILearningPlayground playground;
    playground.Init();

    return playground.Run(snake.weights, snake.biases);
}

std::vector<float> AIManager::ConvertToVectorF(const NNWeights& weights)
{
    std::size_t size = 0;
    for (std::size_t i = 0; i < weights.size() - 1; ++i)
    {
        size += weights[i].size() * weights[i + 1].size();
    }

    std::vector<float> result;
    result.reserve(size);

    for (const auto& layer_weights : weights)
    {
        for (const auto& neuron_weights : layer_weights)
        {
            for (std::size_t i = 0; i < neuron_weights.size(); ++i)
            {
                result.push_back(neuron_weights[i]);
            }
        }
    }

    return std::move(result);
}

NNWeights AIManager::ConvertToWeights(const std::vector<float>& genome, const NNLayersConfig& config)
{
    NNWeights weights;

    for (std::size_t i = 0; i < config.layers_sizes.size() - 1; ++i)
    {
        weights.emplace_back(config.layers_sizes[i], std::vector<float>(config.layers_sizes[i + 1], 0.f));
    }
    weights.emplace_back(config.layers_sizes.back(), std::vector<float>(config.layers_sizes.back(), 0.f));

    std::size_t counter = 0;
    for (auto& layer_weights : weights)
    {
        for (auto& neuron_weights : layer_weights)
        {
            for (std::size_t i = 0; i < neuron_weights.size(); ++i)
            {
                neuron_weights[i] = genome[counter++];
            }
        }
    }

    return std::move(weights);
}

std::vector<float> AIManager::ConvertToVectorF(const std::vector<std::vector<float>>& biases)
{
    std::vector<float> result;

    for (const auto& b : biases)
    {
        for (float bias : b)
        {
            result.push_back(bias);
        }
    }
    return result;
}

std::vector<std::vector<float>> AIManager::ConvertToBiases(const std::vector<float>& biases_f, const NNLayersConfig& config)
{
    std::vector<std::vector<float>> result;

    std::size_t counter = 0;
    for (std::size_t i = 0; i < config.layers_sizes.size(); ++i)
    {
        result.emplace_back(config.layers_sizes[i]);
        for (auto& bias : result[i])
        {
            bias = biases_f[counter++];
        }
    }
    return result;
}

void AIManager::ImGuiUpdate(float)
{
    if (isImGuiOpen)
    {
        ImGui::Begin("Learning", &isImGuiOpen);

        static int generation = 0;
        static std::int64_t best_score = 0;
        static std::int64_t best_lifetime = 0;

        static int training_sycles = 1;

        auto best = ga.GetGenerationBest();

        auto best_weights = ConvertToWeights(best.weights, layerConfig);
        auto best_biases = ConvertToBiases(best.biases, layerConfig);

        ImGui::InputInt("Training sycles", &training_sycles);

        if (ImGui::Button("Run"))
        {
            int count = training_sycles;
            while (count > 0)
            {
                auto fitness = best_lifetime;
                //auto temp_weights = ConvertToWeights(best.weights, layerConfig);
                //auto temp_biases = ConvertToBiases(best.biases, layerConfig);

                while (fitness <= best_lifetime)
                {
                    ga.GenerateNextGeneration();
                    ga.GatherDatasForGeneration();

                    best = ga.GetGenerationBest();
                    best_score = best.data.score;
                    fitness = best.data.lifetime;

                    //temp_weights = ConvertToWeights(best.weights, layerConfig);
                    //temp_biases = ConvertToBiases(best.biases, layerConfig);
                    best_weights = ConvertToWeights(best.weights, layerConfig);
                    best_biases = ConvertToBiases(best.biases, layerConfig);
                    //network->SetWeights(temp_weights);
                    //network->SetBiases(temp_biases);
                    network->SetWeights(best_weights);
                    network->SetBiases(best_biases);
                    ++generation;
                }

                //best_weights = temp_weights;
                //best_biases = temp_biases;
                best_lifetime = fitness;
                --count;
            }
        }

        ImGui::Text("Generation:\t%d", generation);
        ImGui::Text("Best score:\t%d", best_score);
        ImGui::Text("Best lifetime:\t%d", best_lifetime);

        ImGui::End();
    }
}
