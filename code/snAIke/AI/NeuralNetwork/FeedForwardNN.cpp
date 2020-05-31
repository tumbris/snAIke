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

static float relu(float x)
{
    return std::max(0.f, x);
}

Layer::Layer(std::size_t neurons_num, std::size_t next_layer_size)
    : neurons(neurons_num, 0.f)
    , weights(neurons_num, std::vector<float>(next_layer_size, 1.f))
    , biases(neurons_num, 0)
{
}

void Layer::Accept(const Layer& input)
{
    for (std::size_t a_i1 = 0; a_i1 < neurons.size(); ++a_i1)
    {
        neurons[a_i1] = 0;
        for (std::size_t a_i0 = 0; a_i0 < input.neurons.size(); ++a_i0)
        {
            neurons[a_i1] += input.neurons[a_i0] * input.weights[a_i0][a_i1];
        }
        neurons[a_i1] = relu(neurons[a_i1] + biases[a_i1]);
    }
}

std::vector<float> Layer::Activate()
{
    std::vector<float> res(neurons.size());
    for (std::size_t i = 0; i < neurons.size(); ++i)
    {
        res[i] = relu(neurons[i] + biases[i]);
    }
    return res;
}

Layer Layer::FromInput(const std::vector<float>& input)
{
    Layer res(input.size(), input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        res.neurons[i] = input[i];
        res.biases[i] = 0.f;
        std::fill(res.weights.begin(), res.weights.end(), std::vector<float>(input.size(), 1.f));
    }
    return res;
}

void FeedForwardNN::Init(const NNLayersConfig& config)
{
    layers.clear();
    for (std::size_t i = 0; i < config.layers_sizes.size() - 1; ++i)
    {
        layers.emplace_back(config.layers_sizes[i], config.layers_sizes[i + 1]);
    }
    layers.emplace_back(config.layers_sizes.back(), config.layers_sizes.back());
}

std::vector<float> FeedForwardNN::Run(const std::vector<float>& input)
{
    assert(input.size() == layers[0].neurons.size());

    auto input_layer = Layer::FromInput(input);
    layers[0].Accept(input_layer);
    for (std::size_t i = 1; i < layers.size(); ++i)
    {
        layers[i].Accept(layers[i - 1]);
    }
    return layers.back().Activate();
}

void FeedForwardNN::Randomize()
{
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_real_distribution<float> weight_ud(-1.f, 1.f);
    std::uniform_real_distribution<float> biases_ud(-1.f, 1.f);

    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        for (std::size_t j = 0; j < layers[i].weights.size(); ++j)
        {
            for (std::size_t k = 0; k < layers[i].weights[j].size(); ++k)
            {
                layers[i].weights[j][k] = weight_ud(e);
            }
            layers[i].biases[j] = biases_ud(e);
        }
    }
}

void FeedForwardNN::SetWeights(const NNWeights& weights)
{
    assert(weights.size() == layers.size());
    if (weights.size() < layers.size()) return;

    for (std::size_t i = 0; i < layers.size() && i < weights.size(); ++i)
    {
        assert(layers[i].weights.size() == weights[i].size());
        std::copy(weights[i].begin(), weights[i].end(), layers[i].weights.begin());
    }
}

NNWeights FeedForwardNN::GetWeights() const
{
    NNWeights result(layers.size());
    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        result[i].reserve(layers[i].weights.size());
        std::copy(layers[i].weights.begin(), layers[i].weights.end(), std::back_inserter(result[i]));
    }
    return result;
}

void FeedForwardNN::SetBiases(const std::vector<std::vector<float>>& biases)
{
    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        layers[i].biases = biases[i];
    }
}

std::vector<std::vector<float>> FeedForwardNN::GetBiases() const
{
    std::vector<std::vector<float>> result;
    for (const auto& layer : layers)
    {
        result.push_back(layer.biases);
    }
    return result;
}

bool FeedForwardNN::LoadFromFile(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    layers.clear();

    std::string temp;
    {
        if (std::getline(file, temp) && !temp.empty())
        {
            std::istringstream layers_s(temp);
            std::vector<std::size_t> layers_sizes;
            std::size_t temp_s = 0;
            while (layers_s >> temp_s)
            {
                layers_sizes.push_back(temp_s);
            }

            for (std::size_t i = 0; i < layers_sizes.size() - 1; ++i)
            {
                layers.emplace_back(layers_sizes[i], layers_sizes[i + 1]);
            }
            layers.emplace_back(layers_sizes.back(), layers_sizes.back());

            for (auto& layer : layers)
            {
                for (std::size_t i = 0; i < layer.biases.size(); ++i)
                {
                    file >> layer.biases[i];
                }
                for (auto& weight : layer.weights)
                {
                    for (std::size_t i = 0; i < weight.size(); ++i)
                    {
                        file >> weight[i];
                    }
                }
            }
        }
    }
    file.close();
    return true;
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
        for (float bias : layer.biases)
        {
            file << bias << ' ';
        }
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
