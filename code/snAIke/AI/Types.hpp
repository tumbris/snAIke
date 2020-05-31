#pragma once

#include <vector>
#include <cmath>

using LayerWeights = std::vector<std::vector<float>>;

using NNWeights = std::vector<LayerWeights>;

struct AISnakeData
{
    std::int64_t score = 0;
    std::int64_t lifetime = 0;
};

struct AISnake
{
    std::vector<float> weights;
    std::vector<float> biases;
    AISnakeData data;

    double CalculateFitness() const
    {
        double fitness = std::pow(2.0, data.score) + data.lifetime;
        return fitness;
    }
};

struct NNLayersConfig
{
    std::vector<std::size_t> layers_sizes;
};