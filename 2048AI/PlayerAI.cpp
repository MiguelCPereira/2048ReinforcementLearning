#include "PlayerAI.h"
#include <iostream>
#include "GameLogic.h"
#include "GameObject.h"


PlayerAI::PlayerAI(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::GameObject>& gameLogicGObj, float timeBetweenMoves)
	: m_GameObject(gameObject)
	, m_GameLogic()
	, m_TimeBetweenMoves(timeBetweenMoves)
	, m_MoveTimeCounter()

	//, m_PlotScores()
	//, m_PlotMeanScores()
	//, m_TotalScore()
	//, m_Highscore()
	//, m_NrPlayedGames()
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





//void PlayerAI::Train()
//{
//	if (m_GameLogic->GetGameOver() == false)
//	{
//		// Get the current game state and calculate the AI's next move with it
//		const auto oldState = m_GameLogic->GetState();
//		const auto nextMove = CalculateAction(oldState);
//
//		// Play said move and store the reward
//		// The reward returned by the Swipe functions is calculated by adding 1
//		// for each piece collapsed and subtracting 1 for each piece spawned
//		// and subtracting 10 when losing the game
//		int reward;
//		switch (nextMove)
//		{
//		case 1:
//			reward = m_GameLogic->SwipeUp();
//			break;
//		case 2:
//			reward = m_GameLogic->SwipeDown();
//			break;
//		case 3:
//			reward = m_GameLogic->SwipeLeft();
//			break;
//		case 4:
//			reward = m_GameLogic->SwipeRight();
//			break;
//		default:
//			break;
//
//		}
//
//		// And get the new current game state
//		const auto score = m_GameLogic->GetScore();
//		const auto gameOver = m_GameLogic->GetGameOver();
//		const auto newState = m_GameLogic->GetState();
//
//		// Train the short memory and remember
//		TrainShortMemory(oldState, newState, nextMove, reward, gameOver);
//		Remember(oldState, newState, nextMove, reward, gameOver);
//
//		// If this move made the game end
//		if (gameOver)
//		{
//			// Train the long memory and plot the result
//			m_NrPlayedGames++;
//			TrainLongMemory();
//
//			if (score > m_Highscore)
//			{
//				m_Highscore = score;
//				std::cout << "Game " << m_NrPlayedGames << " | Score: " << score << " Record: " << m_Highscore << '\n';
//
//				m_PlotScores.push_back(score);
//				m_TotalScore += score;
//				const auto meanScore = m_TotalScore / m_NrPlayedGames;
//				m_PlotMeanScores.push_back(meanScore);
//				Plot();
//			}
//		}
//	}
//}