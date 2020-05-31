#pragma once

#include <snAIke/AI/Types.hpp>
#include <functional>

class AILearningPlayground;

class GeneticAlgorithm
{
public:
    static constexpr std::size_t populationSize = 1200;
    static constexpr float mutationRate = 0.1f;
public:    
    void GenerateInitialPopulation(const std::vector<float>& init_weights, const std::vector<float>& init_biases);

    void GenerateNextGeneration();
    
    void GatherDatasForGeneration();

    const AISnake& GetGenerationBest() const;

private:
    void Mutate(std::vector<float>& source);
    AISnake Breed(const AISnake& a, const AISnake& b);
    void SortGeneration();

private:
    std::vector<AISnake> currentPopulation;
};
