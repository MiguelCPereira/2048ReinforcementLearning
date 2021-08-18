#include "PlayerAI.h"
#include "GameLogic.h"
#include "GameObject.h"


PlayerAI::PlayerAI(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::GameObject>& gameLogicGObj, float timeBetweenMoves)
	: m_GameObject(gameObject)
	, m_GameLogic()
	, m_TimeBetweenMoves(timeBetweenMoves)
	, m_MoveTimeCounter()
{
	m_GameLogic = gameLogicGObj->GetComponent<GameLogic>();
}

void PlayerAI::Initialize()
{
}

void PlayerAI::Update(const float deltaTime)
{
	// Right now, the AI only plays randomly
	
	m_MoveTimeCounter += deltaTime;
	if (m_MoveTimeCounter >= m_TimeBetweenMoves)
	{
		m_MoveTimeCounter = 0.f;
		const auto randomMove = rand() % 4 + 1;

		switch(randomMove)
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


