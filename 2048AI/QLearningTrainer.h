#pragma once
#include <torch/torch.h> // Uncomment this include for the program to run without crashing (and 2 others on PlayerAI.cpp and LinearQNet.h)
//#define NOMINMAX
#include <vector>
#include "LinearQNet.h"

struct TrainingInfo
{
    std::vector<int>* oldState;
    std::vector<int>* newState;
    int* nextMove;
    int* reward;
    bool gameOver;
};

class QLearningTrainer
{
public:
    QLearningTrainer(LinearQNet* model, float discount, double learningRate);
    ~QLearningTrainer() = default;

    void TrainStep(TrainingInfo* trainingInfo);

private:
    LinearQNet* m_Model;
    const float m_Discount;
	const double m_LearningRate;
    //torch::optim::Adam m_Optimizer;
    //torch::nn::MSELoss m_Criterion;
};

// This class, in its current state, is still majorly influenced by the following video:
// https://www.youtube.com/watch?v=VGkcmBaeAGM
