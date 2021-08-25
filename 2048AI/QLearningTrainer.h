#pragma once
#include <vector>

class NeuralNetwork;

struct TrainingInfo
{
    std::vector<int>* oldState;
    std::vector<int>* newState;
    int* nextMove;
    float* reward;
    bool gameOver;
};

class QLearningTrainer
{
public:
    QLearningTrainer(NeuralNetwork* model, float discount);
    ~QLearningTrainer() = default;

    void TrainStep(TrainingInfo* trainingInfo) const;

private:
    NeuralNetwork* m_Model;
    const float m_Discount;
};
