#include <snAIke/AI/NeuralNetwork/FeedForwardNN.hpp>

#include <sstream>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include <cassert>

static float sigmoid(float x)
{
    return 1.f / (std::expf(-x) + 1);
}

Layer::Layer(std::size_t neurons_num, std::size_t next_layer_size)
    : neurons(neurons_num, 0.f)
    , weights(neurons_num, std::vector<float>(next_layer_size, 0.f))
{
}

void Layer::Pass(std::vector<float>& next)
{
    for (std::size_t a_i1 = 0; a_i1 < next.size(); ++a_i1)
    {
        next[a_i1] = 0;
        for (std::size_t a_i0 = 0; a_i0 < neurons.size(); ++a_i0)
        {
            next[a_i1] += neurons[a_i0] * weights[a_i0][a_i1];
        }
        next[a_i1] = sigmoid(next[a_i1]);
    }
}

void FeedForwardNN::Init(const Config& config)
{
    layers.clear();
    for (std::size_t i = 0; i < config.layers_sizes.size() - 1; ++i)
    {
        layers.emplace_back(config.layers_sizes[i], config.layers_sizes[i + 1]);
    }
    layers.emplace_back(config.layers_sizes.back(), config.outputs_count);
    outputs.resize(config.outputs_count);
}

std::vector<float> FeedForwardNN::Run(const std::vector<float>& input)
{
    assert(input.size() == layers[0].neurons.size());

    { //fill input layer
        for (std::size_t i = 0; i < input.size() && i < layers[0].neurons.size(); ++i)
        {
            layers[0].neurons[i] = input[i];
        }
    }
    { //run inputs through layers
        for (std::size_t i = 0; i < layers.size() - 1; ++i)
        {
            layers[i].Pass(layers[i + 1].neurons);
        }
        layers.back().Pass(outputs);
    }
    return outputs;
}

void FeedForwardNN::SetRandomWeights()
{
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_real_distribution<float> uniform_dist(-1.f, 1.f);

    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        for (std::size_t j = 0; j < layers[i].weights.size(); ++j)
        {
            for (std::size_t k = 0; k < layers[i].weights[j].size(); ++k)
            {
                layers[i].weights[j][k] = uniform_dist(e);
            }
        }
    }
}

void FeedForwardNN::SetWeights(const std::vector<Layer::WeightMat>& weights)
{
    assert(weights.size() == layers.size());
    if (weights.size() < layers.size()) return;

    for (std::size_t i = 0; i < layers.size() && i < weights.size(); ++i)
    {
        assert(layers[i].weights.size() == weights[i].size());
        std::copy(weights[i].begin(), weights[i].end(), layers[i].weights.begin());
    }
}

std::vector<Layer::WeightMat> FeedForwardNN::GetWeights() const
{
    std::vector<Layer::WeightMat> result(layers.size());
    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        result[i].reserve(layers[i].weights.size());
        std::copy(layers[i].weights.begin(), layers[i].weights.end(), std::back_inserter(result[i]));
    }
    return result;
}

bool FeedForwardNN::LoadFromFile(std::string filename)
{
    return false;
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    //std::string temp;
    //float w = 0.f;
    //{ //input layer
    //    if (std::getline(file, temp) && !temp.empty())
    //    {
    //        std::istringstream output_layer_ss(temp);
    //        while (output_layer_ss >> w)
    //        {
    //            input_layer.neurons.push_back({ w });
    //        }
    //    }
    //}
    //{ //hidden layers
    //    while (std::getline(file, temp) && !temp.empty())
    //    {
    //        hidden_layers.emplace_back();
    //        std::istringstream hidden_layer_ss(temp);
    //        while (hidden_layer_ss >> w)
    //        {
    //            input_layer.neurons.push_back({ w });
    //        }
    //    }
    //}
    //return true;
}

void FeedForwardNN::SaveToFile(std::string filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (auto& layer : layers)
    {
        file << layer.weights.size() << ' ';
    }
    file << '\n';

    for (const auto& layer : layers)
    {
        for (const auto& weights : layer.weights)
        {
            for (std::size_t i = 0; i < weights.size(); ++i)
            {
                file << weights[i] << ' ';
            }
            file << '\n';
        }
    }

    file.close();
}
