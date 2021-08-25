#include "NeuralNetwork.h"
#include <fstream>
#include <string>

// This class is highly influenced by the one described in this website:
// https://www.geeksforgeeks.org/ml-neural-network-implementation-in-c-from-scratch/

//NeuralNetwork::NeuralNetwork(std::vector<unsigned int> topology, float learningRate)
NeuralNetwork::NeuralNetwork(unsigned int inputSize, unsigned int hiddenSize, unsigned int outputSize, float learningRate)
	: m_Topology({inputSize, hiddenSize, outputSize})
	, m_LearningRate(learningRate)
	, m_NeuronLayers()
	, m_CacheLayers()
	, m_NeuronError()
	, m_ConnectionWeights()
{
    for (unsigned int i = 0; i < m_Topology.size(); i++)
    {
        // Initialize all the neuron layers with the given nr of neurons
    	// Aside from the last layer (the output layer), which should have 1 extra bias neuron
        if (i == m_Topology.size() - 1)
            m_NeuronLayers.push_back(new RowVector(m_Topology[i]));
        else
            m_NeuronLayers.push_back(new RowVector(m_Topology[i] + 1));

        // Initialize cache and delta vectors with the same size as the neuron layers one
        m_CacheLayers.push_back(new RowVector(m_NeuronLayers.size()));
        m_NeuronError.push_back(new RowVector(m_NeuronLayers.size()));

        // To all layers aside from the last one
        if (i != m_Topology.size() - 1)
        {
            // Get a reference to the last added neuron and cache layers
			// So they're initialized with 1
            m_NeuronLayers.back()->coeffRef(m_Topology[i]) = 1;
            m_CacheLayers.back()->coeffRef(m_Topology[i]) = 1;
        }

        // Initialize the connection weights matrix
        if (i > 0) // Ignoring the first neuron layer, because that's the input one
        {
            if (i != m_Topology.size() - 1) // To every layer aside the last one
            {
            	// Calculate the current weight matrix by doing a product of the previous neuron layer and the current one
                m_ConnectionWeights.push_back(new Matrix(m_Topology[i - 1] + 1, m_Topology[i] + 1));
            	// Then randomize its values
                m_ConnectionWeights.back()->setRandom();
            	// Set the column to zero
                m_ConnectionWeights.back()->col(m_Topology[i]).setZero();
            	// And set the connection between the last layer's neuron and the current one to 1
                m_ConnectionWeights.back()->coeffRef(m_Topology[i - 1], m_Topology[i]) = 1;
            }
            else // For the last layer
            {
            	// Calculate the weight as previously, but don't add a bias neuron to the current layer
                m_ConnectionWeights.push_back(new Matrix(m_Topology[i - 1] + 1, m_Topology[i]));
                // And also randomize its values
                m_ConnectionWeights.back()->setRandom();
            }
        }
    }
};

RowVector NeuralNetwork::PropagateForward(RowVector& input)
{
    // Set the 1st layer (the input layer) with the given input
    m_NeuronLayers.front()->block(0, 0, 1, m_NeuronLayers.front()->size() - 1) = input;

    // Propagate the data forward
	// By making each neuron layer the matrix dot product of the previous neuron layer and its respective weight
    for (unsigned int i = 1; i < m_Topology.size(); i++)
        (*m_NeuronLayers[i]) = (*m_NeuronLayers[i - 1]) * (*m_ConnectionWeights[i - 1]);

    // Apply the activation function to each layer of the network
    for (unsigned int i = 1; i < m_Topology.size() - 1; i++)
    {
        for (unsigned int col = 0; col < m_Topology[i]; col++)
		{
			// Update the connection's weight with the recorded errors from each neuron
            m_NeuronLayers[i]->coeffRef(0, col) = ActivationFunction(m_NeuronLayers[i]->coeffRef(0, col));
		}
    }

	// Return the output layer
    return m_NeuronLayers.back()->block(0, 0, 1, m_NeuronLayers.back()->size());
}

void NeuralNetwork::PropagateBackward(RowVector& output)
{
    CalculateErrors(output);
    UpdateWeights();
}

void NeuralNetwork::CalculateErrors(RowVector& output)
{
    // Calculate the errors made from the neurons of the last layer
    (*m_NeuronError.back()) = output - (*m_NeuronLayers.back());

    // For the hidden layers, go one by one from the last to the first one
    for (unsigned int i = m_Topology.size() - 2; i > 0; i--)
        (*m_NeuronError[i]) = (*m_NeuronError[i + 1]) * (m_ConnectionWeights[i]->transpose());
}

void NeuralNetwork::UpdateWeights()
{
    // Iterate from the first hidden layer to the output one
    for (unsigned int i = 0; i < m_Topology.size() - 1; i++)
    {
        if (i != m_Topology.size() - 2) // If this is one of the hidden layers
        {
            for (unsigned int col = 0; col < m_ConnectionWeights[i]->cols() - 1; col++) // Count for the bias neuron
            {
                for (unsigned int row = 0; row < m_ConnectionWeights[i]->rows(); row++)
                {
                    // Update the connection's weight with the recorded errors from each neuron
                    m_ConnectionWeights[i]->coeffRef(row, col) += m_LearningRate * m_NeuronError[i + 1]->coeffRef(col) * ActivationFunctionDerivative(m_CacheLayers[i + 1]->coeffRef(col)) * m_NeuronLayers[i]->coeffRef(row);
                }
            }
        }
        else // If this is the output layer
        {
            for (unsigned int col = 0; col < m_ConnectionWeights[i]->cols(); col++) // There is no bias neuron
            {
                for (unsigned int row = 0; row < m_ConnectionWeights[i]->rows(); row++)
                {
                    // Update the connection's weight with the recorded errors from each neuron
                    m_ConnectionWeights[i]->coeffRef(row, col) += m_LearningRate * m_NeuronError[i + 1]->coeffRef(col) * ActivationFunctionDerivative(m_CacheLayers[i + 1]->coeffRef(col)) * m_NeuronLayers[i]->coeffRef(row);
                }
            }
        }
    }
}

float NeuralNetwork::ActivationFunction(float x)
{
	// Get the hyperbolic tangent of the neuron's value
    return tanhf(x);
}

float NeuralNetwork::ActivationFunctionDerivative(float x)
{
    return 1 - tanhf(x) * tanhf(x);
}