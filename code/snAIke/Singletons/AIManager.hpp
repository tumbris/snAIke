#pragma once

#include <snAIke/Singletons/Singleton.hpp>
#include <snAIke/AI/NeuralNetwork/FeedForwardNN.hpp>
#include <snAIke/AI/Learning/GeneticAlgorithm.hpp>
#include <snAIke/Singletons/Director/Types.hpp>

class AISnakeController;

#include <snAIke/Utility/Ref/IntrusivePtr.hpp>

class AIManager
{
private:
    AIManager();
    ~AIManager();

public:
    const NNLayersConfig& GetLayersConfig() const { return layerConfig; }

    AISnakeData GenerateDataForSnakeThreaded(const AISnake& snake);

    static std::vector<float> ConvertToVectorF(const NNWeights& weights);
    static NNWeights ConvertToWeights(const std::vector<float>& genome, const NNLayersConfig& config);

    static std::vector<float> ConvertToVectorF(const std::vector<std::vector<float>>& biases);
    static std::vector<std::vector<float>> ConvertToBiases(const std::vector<float>& biases_f, const NNLayersConfig& config);

    bool IsImGuiShowing() const { return isImGuiOpen; }

    void OpenImGui() { isImGuiOpen = true; }
    void HideImGui() { isImGuiOpen = false; }

private:
    NNLayersConfig layerConfig;
    IntrusivePtr<FeedForwardNN> network;
    IntrusivePtr<AISnakeController> aiController;
    GeneticAlgorithm ga;

    UpdatorIdType imGuiUpdatorCallbackId = -1;

    bool isImGuiOpen = false;
private:
    void ImGuiUpdate(float);
private:
    friend class Singleton<AIManager>;
};