#include <snAIke/AI/Learning/GeneticAlgorithm.hpp>

#include <snAIke/AI/Learning/AILearningPlayground.hpp>

#include <snAIke/Singletons/AIManager.hpp>
#include <snAIke/Utility/Random.hpp>

#include <algorithm>
#include <future>

#include <omp.h>


void GeneticAlgorithm::GenerateInitialPopulation(const std::vector<float>& init_weights, const std::vector<float>& init_biases)
{
    currentPopulation.clear();
    currentPopulation.reserve(populationSize);

    for (std::size_t i = 0; i < populationSize; ++i)
    {
        AISnake temp{ init_weights, init_biases, { 0, 0 } };
        Mutate(temp.weights);
        Mutate(temp.biases);
        currentPopulation.emplace_back(std::move(temp));
    }
}

void GeneticAlgorithm::GenerateNextGeneration()
{
    constexpr static std::uint32_t outspring_rate = 100;

    std::vector<AISnake> new_generation;
    new_generation.reserve(currentPopulation.size());

    std::size_t generation_best = currentPopulation.size() / outspring_rate;
    for (std::size_t i = 0; i < generation_best; ++i)
    {
        AISnake temp{ currentPopulation[i].weights, currentPopulation[i].biases, { } };
        new_generation.emplace_back(std::move(temp));
    }

    for (std::size_t i = 0; i < generation_best / 2; ++i)
    {
        for (std::size_t j = 0; j < outspring_rate - 1; ++j)
        {
            AISnake a = Breed(currentPopulation[i], currentPopulation[i + 1]);
            AISnake b = Breed(currentPopulation[i + 1], currentPopulation[i]);

            new_generation.emplace_back(std::move(a));
            new_generation.emplace_back(std::move(b));
        }
    }

    if (currentPopulation.size() != new_generation.size()) __debugbreak();
    currentPopulation = new_generation;
}

void GeneticAlgorithm::GatherDatasForGeneration()
{
    std::int64_t count = currentPopulation.size();
#pragma omp parallel for
    for (std::int64_t i = 0; i < count; ++i)
    {
        currentPopulation[i].data = Singleton<AIManager>::GetInstance()->GenerateDataForSnakeThreaded(currentPopulation[i]);
    }

    SortGeneration();
}

const AISnake& GeneticAlgorithm::GetGenerationBest() const
{
    return *std::max_element(currentPopulation.begin(), currentPopulation.end(),
                             [](const AISnake& a, const AISnake& b)
                             {
                                 return a.CalculateFitness() < b.CalculateFitness();
                             });
}

void GeneticAlgorithm::Mutate(std::vector<float>& source)
{
    std::transform(source.begin(), source.end(), source.begin(),
                   [](float gene)
                   {
                       float mutation_chance = RandInRange(0.f, 1.f);
                       if (mutation_chance < mutationRate)
                       {
                           gene += RandInRange(-1.f, 1.f);
                       }
                       return gene;
                   });
}

AISnake GeneticAlgorithm::Breed(const AISnake& a, const AISnake& b)
{
    std::size_t crossover_point = RandInRange(a.weights.size() / 4, a.weights.size() - a.weights.size() / 4);
    std::vector<float> resulting_weights(a.weights.size());
    for (std::size_t i = 0; i < crossover_point; ++i)
    {
        resulting_weights[i] = a.weights[i];
    }
    for (std::size_t i = crossover_point; i < b.weights.size(); ++i)
    {
        resulting_weights[i] = b.weights[i];
    }
    Mutate(resulting_weights);

    crossover_point = RandInRange(a.biases.size() / 4, a.biases.size() - a.biases.size() / 4);
    std::vector<float> resulting_biases(a.biases.size());
    for (std::size_t i = 0; i < crossover_point; ++i)
    {
        resulting_biases[i] = a.biases[i];
    }
    for (std::size_t i = crossover_point; i < b.biases.size(); ++i)
    {
        resulting_biases[i] = b.biases[i];
    }
    Mutate(resulting_biases);

    return AISnake{ resulting_weights, resulting_biases, {0, 0} };
}

void GeneticAlgorithm::SortGeneration()
{
    std::sort(currentPopulation.begin(), currentPopulation.end(),
              [](const AISnake& a, const AISnake& b)
              {
                  return a.CalculateFitness() > b.CalculateFitness();
              });
}
