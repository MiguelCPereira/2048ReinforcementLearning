#include "QLearningTrainer.h"

QLearningTrainer::QLearningTrainer(LinearQNet* model, float discount, double learningRate)
	: m_Model(model)
	, m_Discount(discount)
	, m_LearningRate(learningRate)
	//, m_Optimizer(model->parameters(), learningRate)
	//, m_Criterion()
{
}

void QLearningTrainer::TrainStep(TrainingInfo* /*trainingInfo*/)
{
    /*// Turn all the TrainingInfo members into tensors (aside from the game over bool)
    const auto optionsFloat = torch::TensorOptions().dtype(torch::kFloat64);
    const auto optionsLong = torch::TensorOptions().dtype(torch::kLong);
    auto oldStateTensor = torch::from_blob(trainingInfo->oldState, { 16 }, optionsFloat);
    auto newStateTensor = torch::from_blob(trainingInfo->newState, { 16 }, optionsFloat);
	auto nextMoveTensor = torch::from_blob(trainingInfo->nextMove, { 4 }, optionsLong);
    auto rewardTensor = torch::from_blob(trainingInfo->reward, { 1 }, optionsFloat);

	// Unsqueeze all tensors
    oldStateTensor = torch::unsqueeze(oldStateTensor, 0);
    newStateTensor = torch::unsqueeze(newStateTensor, 0);
    nextMoveTensor = torch::unsqueeze(rewardTensor, 0);
    rewardTensor = torch::unsqueeze(rewardTensor, 0);

	// Predict the QValues through the LinearQNet
    auto prediction = m_Model->Forward(oldStateTensor);

    auto target = prediction.clone();

	if(trainingInfo->gameOver == false) // If the stored move didn't lead to a game over
	{
        // Calculate the best move considering not only the current reward, but the possible reward of the next state as well
		
		// Do so with the QLearning algorithm
        const auto qNextStateTensor = torch::max(m_Model->Forward(newStateTensor));
        const auto qState = rewardTensor + m_Discount * qNextStateTensor;
        target[0][torch::argmax(nextMoveTensor[0])] = qState;

		// Calculate the loss function
        m_Optimizer.zero_grad();
        const auto loss = m_Criterion(target, prediction);
        loss.backward();
        m_Optimizer.step();
	}
    else // If it's the last move
    {
    	// Just make the next move the one with the best immediate reward
        target[0][torch::argmax(nextMoveTensor[0])] = rewardTensor;
    }*/
}
