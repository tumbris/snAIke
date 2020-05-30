#pragma once

#include <snAIke/Utility/Ref/ReferenceCountable.hpp>

#include <vector>
#include <string>

struct Layer
{
    using WeightMat = std::vector<std::vector<float>>;

    Layer(std::size_t neurons_num, std::size_t next_layer_size);
    
    void Pass(std::vector<float>& next);

    std::vector<float> neurons;
    WeightMat weights;
};

class FeedForwardNN : public ReferenceCountable<DefaultThreadPolicy>
{
public:
    struct Config
    {
        std::vector<std::size_t> layers_sizes;
        std::size_t outputs_count;
    };

public:
    void Init(const Config& config);

    std::vector<float> Run(const std::vector<float>& input);

    void SetRandomWeights();

    void SetWeights(const std::vector<Layer::WeightMat>& weights);
    std::vector<Layer::WeightMat> GetWeights() const;

    bool LoadFromFile(std::string filename = "nncfg.txt");
    void SaveToFile(std::string filename = "nncfg.txt");

private:
    std::vector<Layer> layers;
    std::vector<float> outputs;
};

