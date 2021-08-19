#pragma once
#include <string>
#include <torch/torch.h>

class LinearQNet : torch::nn::Module
{
public:
    LinearQNet(int inputSize, int hiddenSize, int outputSize);

    torch::Tensor Forward(torch::Tensor x);
    void Save(std::string fileName = "model.pth") const;

private:
	torch::nn::Linear m_LinearA;
    torch::nn::Linear m_LinearB;
};

// This class is still incomplete, but its current state is majorly influenced by the following video:
// https://www.youtube.com/watch?v=VGkcmBaeAGM

