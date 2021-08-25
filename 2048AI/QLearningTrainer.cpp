#include "QLearningTrainer.h"
#include "NeuralNetwork.h"

QLearningTrainer::QLearningTrainer(NeuralNetwork* model, float discount)
	: m_Model(model)
	, m_Discount(discount)
{
}

void QLearningTrainer::TrainStep(TrainingInfo* trainingInfo) const
{
	
    // Create a row vector from the old state values
    Eigen::RowVectorXf oldStateRowVec(trainingInfo->oldState->size());
	for (int i = 0; i < trainingInfo->oldState->size(); i++)
        oldStateRowVec.coeffRef(0, i) = trainingInfo->oldState->operator[](i);

    // Create a row vector from the new state values
    Eigen::RowVectorXf newStateRowVec(trainingInfo->newState->size());
    for (int i = 0; i < trainingInfo->newState->size(); i++)
        oldStateRowVec.coeffRef(0, i) = trainingInfo->newState->operator[](i);

    //Create a row vector from the reward
    Eigen::RowVectorXf rewardRowVec(4);
    for (int i = 0; i < 4; i++)
    {
        if (i == *trainingInfo->nextMove)
            rewardRowVec.coeffRef(0, i) = *trainingInfo->reward;
        else
            rewardRowVec.coeffRef(0, i) = 0.f;
    }

    // Get the output of the old state predicted by the Neural Network
    auto prediction = m_Model->PropagateForward(oldStateRowVec);

	// If the stored move didn't lead to a game over, adjust the prediction with the QLearning algorithm
    auto optimalMoveRowVec = rewardRowVec;
    if (trainingInfo->gameOver == false)
    {
        // First, calculate the predicted move for the newState
        const auto futurePrediction = m_Model->PropagateForward(newStateRowVec);

    	// Then calculate the best move, considering not only the current reward,
    	// but also the prediction of the move that will follow the next state
        optimalMoveRowVec = rewardRowVec + m_Discount * futurePrediction;
    }

	// And propagate the new prediction backwards
    m_Model->PropagateBackward(optimalMoveRowVec);
}