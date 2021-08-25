#include "PlayerAI.h"
#include <algorithm>
#include <iostream>
#include "GameLogic.h"
#include "GameObject.h"
#include "LinearQNet.h"
#include "NeuralNetwork.h"


PlayerAI::PlayerAI(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::GameObject>& gameLogicGObj, float timeBetweenMoves)
	: m_GameObject(gameObject)
	, m_GameLogic()
	, m_TimeBetweenMoves(timeBetweenMoves)
	, m_MoveTimeCounter()
	, m_AllScores()
	, m_Highscore()
	, m_NrPlayedGames()

	, m_LearningRate(0.001f)
	, m_RandomFactor(100) // Decreasing amount of random moves the AI will make for exploration's sake
	, m_MinRandomAmount(20) // The minimum amount the random factor can decrease to
	, m_Discount(0.9f) // The percentage in which the QLearningTrainer algorithm will take the future reward in account compared to the current reward
	, m_MaxMemory(100000) // Max amount of possibly stored moves in the m_Memory container
	, m_TrainingBatchSize(1000) // Amount of moves to sample from m_Memory to train in batches
	, m_Memory()
	, m_Model()
	, m_Trainer()
{
	m_GameLogic = gameLogicGObj->GetComponent<GameLogic>();
	// 16 inputs for each square in the board, an approximated hiddenSize of 256 and 4 possible outputs for each swipe type
	m_Model = new NeuralNetwork(16, 256, 4, m_LearningRate);
	m_Trainer = new QLearningTrainer(m_Model, m_Discount);
}

void PlayerAI::Initialize()
{
}

void PlayerAI::Update(const float deltaTime)
{
	// Right now, the AI only plays randomly
	if (m_GameLogic->GetGameOver() == false)
	{
		if (m_TimeBetweenMoves > 0.f)
		{
			m_MoveTimeCounter += deltaTime;
			if (m_MoveTimeCounter >= m_TimeBetweenMoves)
			{
				m_MoveTimeCounter = 0.f;
				MakeNextMove();
			}
		}
		else
		{
			MakeNextMove();
		}
	}
}



int PlayerAI::CalculateAction(std::vector<int>* oldState) const
{
	auto epsilon = m_RandomFactor - m_NrPlayedGames;
	if (epsilon < m_MinRandomAmount)
		epsilon = m_MinRandomAmount;
	
	int finalMove = 1;

	// Progressively smaller chance of a random action (exploration)
	// In the beginning, an 1/2 chance, and then decreasing until the 100th game, where it tanks at 1/10
	
	if (rand() % 201 < epsilon) // If the epsilon's bigger, make the final move random
	{
		finalMove = rand() % 4 + 1;
	}
	else // If not, make the move a prediction from the m_Model
	{
		// Create a row vector from the old state values
		Eigen::RowVectorXf oldStateRowVec(oldState->size());
		for (int i = 0; i < oldState->size(); i++)
			oldStateRowVec.coeffRef(0, i) = oldState->operator[](i);
		
		const auto predictedMoveRowVec = m_Model->PropagateForward(oldStateRowVec);
		float maxValue = oldStateRowVec.coeffRef(0, 0);
		for (int i = 0; i < predictedMoveRowVec.size(); i++)
		{
			if(oldStateRowVec.coeffRef(0, i) > maxValue)
			{
				maxValue = oldStateRowVec.coeffRef(0, i);
				finalMove = i + 1;
			}
			//std::cout << oldStateRowVec.coeffRef(0, i) << " ";
		}
		//std::cout << "-> final move: " << finalMove << "\n";
	}

	return finalMove;
}

void PlayerAI::Remember(TrainingInfo* trainingInfo)
{
	// Pop_back m_Memory deque if the size has reached m_MaxMemory
	if (m_Memory.size() >= m_MaxMemory)
		m_Memory.pop_back();

	// And then save the new training info
	m_Memory.push_back(trainingInfo);
}

void PlayerAI::TrainStepWithSample()
{
	std::deque<TrainingInfo*> sample;
	std::vector<int> sampleIdx;

	if(m_Memory.size() >= m_TrainingBatchSize) // If the memory has enough to collect a sample of the pre-defined batch-size
	{
		// Get all the m_Memory indexes in an int vector
		for (auto i = 0; i < m_Memory.size(); ++i)
			sampleIdx.push_back(i);
		
		// Shuffle all the new vector
		std::random_shuffle(sampleIdx.begin(), sampleIdx.end());
		
		// And then collect a batch-sized sample of TrainingInfos from the memory correspondent to the shuffled indexes
		for (size_t i = 0; i < m_TrainingBatchSize; ++i)
			sample.push_back(m_Memory[sampleIdx[i]]);
	}
	else // If not
	{
		// Just use all the info in the memory as a sample
		sample = m_Memory;
	}

	// And then call the trainStep function for all the sampled TrainingInfos
	for(auto* trainingInfo : sample)
		m_Trainer->TrainStep(trainingInfo);
}

void PlayerAI::MakeNextMove()
{
	// Get the current game state and calculate the AI's next move with it
	auto* oldState = m_GameLogic->GetGameState();
	const auto nextMove = CalculateAction(oldState);

	// Play said move and store the reward
	// The reward returned by the Swipe functions is calculated by adding 1
	// for each piece collapsed and subtracting 1 for each wasted move
	// and subtracting 10 when losing the game
	auto reward = 0;
	switch (nextMove)
	{
	case 1:
		reward = m_GameLogic->SwipeUp();
		break;
	case 2:
		reward = m_GameLogic->SwipeDown();
		break;
	case 3:
		reward = m_GameLogic->SwipeLeft();
		break;
	case 4:
		reward = m_GameLogic->SwipeRight();
		break;
	default:
		break;
	}

	// And get the new current game state, score and gameOver
	auto* newState = m_GameLogic->GetGameState();
	const auto score = m_GameLogic->GetScore();
	const auto gameOver = m_GameLogic->GetGameOver();

	// Train the AI with this move's info
	auto* pNextMove = new int(nextMove);
	auto* pReward = new float(reward);
	auto* trainingInfo = new TrainingInfo{ oldState, newState, pNextMove, pReward, gameOver };
	m_Trainer->TrainStep(trainingInfo);

	// And save the move in the m_Memory container
	Remember(trainingInfo);

	// If this move made the game end
	if (gameOver)
	{
		// Increase the played games counter
		m_NrPlayedGames++;
		
		// Train step with a sample of moves from the m_Memory container
		TrainStepWithSample();

		// If it's a new highscore, update the member variable and save the model
		if (score > m_Highscore)
		{
			m_Highscore = score;
			std::cout << "NEW HIGHSCORE!!\n";
		}

		// Add the new score to the vector
		m_AllScores.push_back(score);

		// Calculate the mean
		int mean = 0;
		for (auto sco : m_AllScores)
			mean += sco;
		mean /= m_NrPlayedGames;

		// And finally print out the game's info to the console
		std::cout << "Game " << m_NrPlayedGames << " | Score: " << score << " Record: " << m_Highscore << " | Mean: " << mean << '\n';
	}
}