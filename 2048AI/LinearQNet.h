#pragma once
#include <string>
#include <torch/torch.h> // Uncomment this include for the program to run without crashing (and 2 others on PlayerAI.cpp and QLearningTrainer.h)
//#define NOMINMAX

class LinearQNet /*: public torch::nn::Module*/
{
	
public:
    LinearQNet(int inputSize, int hiddenSize, int outputSize);
    ~LinearQNet() = default;

    //torch::Tensor Forward(torch::Tensor x);
    void Save(std::string fileName = "model.pth") const;

private:
	//torch::nn::Linear m_LinearA;
    //torch::nn::Linear m_LinearB;
};

// This class, in its current state, is still majorly influenced by the following video:
// https://www.youtube.com/watch?v=VGkcmBaeAGM

