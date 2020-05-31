#pragma once

#include <snAIke/Utility/Ref/ReferenceCountable.hpp>
#include <snAIke/AI/Types.hpp>

#include <vector>
#include <string>

struct Layer
{
    Layer(std::size_t neurons_num, std::size_t next_layer_size);
    
    void Accept(const Layer& inputs);
    std::vector<float> Activate();

    static Layer FromInput(const std::vector<float>& input);

    std::vector<float> neurons;
    std::vector<float> biases;
    LayerWeights weights;
};

class FeedForwardNN : public ReferenceCountable<DefaultThreadPolicy>
{
public:
    void Init(const NNLayersConfig& config);

    std::vector<float> Run(const std::vector<float>& input);

    void Randomize();

    void SetWeights(const NNWeights& weights);
    NNWeights GetWeights() const;

    void SetBiases(const std::vector<std::vector<float>>& biases);
    std::vector<std::vector<float>> GetBiases() const;

    bool LoadFromFile(std::string filename = "nncfg.txt");
    void SaveToFile(std::string filename = "nncfg.txt");

private:
    std::vector<Layer> layers;
};

