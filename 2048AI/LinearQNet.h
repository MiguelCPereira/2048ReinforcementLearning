#pragma once
#include <string>
//#include <torch/torch.h>


// This is an unused class, reminiscent of a first implementation of the Neural Network concept dependent on TorchLib
// (a 3rd party Machine Learning framework). Duo to some issues I had with implementing said library, I ended up scratching it

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
