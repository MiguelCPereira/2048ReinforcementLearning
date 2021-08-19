#include "PlayerAI.h"

#include <algorithm>
#include <iostream>
#include "GameLogic.h"
#include "GameObject.h"


PlayerAI::PlayerAI(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::GameObject>& gameLogicGObj, float timeBetweenMoves)
	: m_GameObject(gameObject)
	, m_GameLogic()
	, m_TimeBetweenMoves(timeBetweenMoves)
	, m_MoveTimeCounter()

	, m_PlotScores()
	, m_PlotMeanScores()
	, m_TotalScore()
	, m_Highscore()
	, m_NrPlayedGames()

	, m_RandomFactor(80)
	, m_Gamma(0.9f) // Discount rate
	, m_MaxMemory(10000/*0*/)
	, m_TrainingBatchSize(100/*0*/)
	, m_Memory()
	//, m_Model = Linear_QNet(11, 256, 3)
	//, m_Trainer(self.model, lr = LR, gamma = self.gamma)
{
	m_GameLogic = gameLogicGObj->GetComponent<GameLogic>();
}

void PlayerAI::Initialize()
{
}

void PlayerAI::Update(const float deltaTime)
{
	// Right now, the AI only plays randomly
	if (m_GameLogic->GetGameOver() == false)
	{
		m_MoveTimeCounter += deltaTime;
		if (m_MoveTimeCounter >= m_TimeBetweenMoves)
		{
			m_MoveTimeCounter = 0.f;
			//Train();

			const auto randomMove = rand() % 4 + 1;

			switch (randomMove)
			{
			case 1:
				m_GameLogic->SwipeUp();
				break;
			case 2:
				m_GameLogic->SwipeDown();
				break;
			case 3:
				m_GameLogic->SwipeLeft();
				break;
			case 4:
				m_GameLogic->SwipeRight();
				break;
			default:
				break;

			}
		}
	}
}



int PlayerAI::CalculateAction(const std::vector<int>& oldState) const
{
	const auto epsilon = m_RandomFactor - m_NrPlayedGames;
	int finalMove = 0;

	// Progressively smaller chance of a random action (exploration)
	// In the beginning, an 80/200 chance, and then decreasing until the 80th game, where the forced exploration stops
	
	if (rand() % 201 < epsilon) // If randomizing
	{
		finalMove = rand() % 5;
	}
	else // If not
	{
		// Need to find a torch replacement for C++
		
		//auto tensorState = torch.tensor(state, dtype = torch.float);
		//auto prediction = m_Model.Predict(tensorState);
		//finalMove = torch.argmax(prediciton).item();
	}

	return finalMove;
}

void PlayerAI::Remember(const TrainingInfo& trainingInfo)
{
	// Pop_back m_Memory deque if the size has reached m_MaxMemory
	if (m_Memory.size() >= m_MaxMemory)
		m_Memory.pop_back();

	// And then save the new training info
	m_Memory.push_back(trainingInfo);
}

void PlayerAI::TrainShortMemory(const TrainingInfo& trainingInfo)
{
	//m_Trainer.TrainStep(trainingInfo);
}

void PlayerAI::TrainLongMemory()
{
	std::deque<TrainingInfo> sample;
	std::vector<int> sampleIdx;
	
	if(m_Memory.size() >= m_TrainingBatchSize)
	{
		for (size_t i = 0; i < m_Memory.size(); ++i) { sampleIdx.push_back(i); }
		std::random_shuffle(sampleIdx.begin(), sampleIdx.end());
		for (size_t i = 0; i < m_TrainingBatchSize; ++i) { sample.push_back(m_Memory[sampleIdx[i]]); }
	}
	else
	{
		sample = m_Memory;
	}

	// Need to divide TrainingInfo into individual containers of OldStates, NewStates, NextMoves, etc
	// But maybe it's better to do it in the m_Trainer function
	// m_Trainer.TrainStep(sample);

	// Oooooooooooooor

	// I can just do this
	//for(auto trainingInfo : sample)
	//	m_Trainer.TrainStep(trainingInfo);
}

void PlayerAI::Plot()
{
}

void PlayerAI::Train()
{
	if (m_GameLogic->GetGameOver() == false)
	{
		// Get the current game state and calculate the AI's next move with it
		const auto oldState = m_GameLogic->GetGameState();
		const auto nextMove = CalculateAction(oldState);

		// Play said move and store the reward
		// The reward returned by the Swipe functions is calculated by adding 1
		// for each piece collapsed and subtracting 1 for each piece spawned
		// and subtracting 10 when losing the game
		int reward = 0;
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

		// And get the new current game state
		const auto score = m_GameLogic->GetScore();
		const auto gameOver = m_GameLogic->GetGameOver();
		const auto newState = m_GameLogic->GetGameState();

		// Train the short memory and remember
		const TrainingInfo trainingInfo = { oldState, newState, nextMove, reward, gameOver };
		TrainShortMemory(trainingInfo);
		Remember(trainingInfo);

		// If this move made the game end
		if (gameOver)
		{
			// Train the long memory and plot the result
			m_NrPlayedGames++;
			TrainLongMemory();

			if (score > m_Highscore)
			{
				m_Highscore = score;
				std::cout << "Game " << m_NrPlayedGames << " | Score: " << score << " Record: " << m_Highscore << '\n';
				// m_Model.save();
				
				m_PlotScores.push_back(score);
				m_TotalScore += score;
				const auto meanScore = m_TotalScore / m_NrPlayedGames;
				m_PlotMeanScores.push_back(meanScore);
				Plot();
			}
		}
	}
}