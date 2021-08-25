#pragma once
#include <Eigen/Dense>
#include <iostream>
#include <vector>

// This class is highly influenced by the one described in this website:
// https://www.geeksforgeeks.org/ml-neural-network-implementation-in-c-from-scratch/

typedef Eigen::MatrixXf Matrix;
typedef Eigen::RowVectorXf RowVector;
typedef Eigen::VectorXf ColVector;

class NeuralNetwork
{
public:
    //NeuralNetwork(std::vector<unsigned int> topology, float learningRate = 0.005f);
    NeuralNetwork(unsigned int inputSize, unsigned int hiddenSize, unsigned int outputSize, float learningRate = 0.005f);

    // Forward the propagation of data
    RowVector PropagateForward(RowVector& input);

    // Backward propagation of errors made by neurons
    void PropagateBackward(RowVector& output);

    // Calculate errors made by neurons in each layer
    void CalculateErrors(RowVector& output);

    // Update the m_ConnectionWeights of connections
    void UpdateWeights();

    // Activate neurons
    float ActivationFunction(float x);
    float ActivationFunctionDerivative(float x);

private:
    std::vector<unsigned int> m_Topology; // The size of this vector corresponds to the amount of layers, and the unsigned int to the nr of neurons in it
    float m_LearningRate;
    std::vector<RowVector*> m_NeuronLayers; // The different layers of the network
    std::vector<RowVector*> m_CacheLayers; // The unactivated values of the layers, aka the sum of weighted inputs from the previous layer
    std::vector<RowVector*> m_NeuronError; // The error contribution of each neuron
    std::vector<Matrix*> m_ConnectionWeights; // The connection m_ConnectionWeights
};