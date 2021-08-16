#include "GameLogic.h"

#include <iostream>

#include "Factory.h"
#include "GameObject.h"
#include "NumberSquare.h"
#include "SceneManager.h"
#include "Scene.h"

GameLogic::GameLogic(const std::shared_ptr<dae::GameObject>& gameObject)
	: m_GameObject(gameObject)
	, m_NumberSquaresVector()
	, m_FreeSquares()
	, m_Score()
{
	Initialize();
}

void GameLogic::Initialize()
{
	// Initialize the vector
	m_NumberSquaresVector = new std::vector<std::shared_ptr<dae::GameObject>>;
	
	// Fill the FreeSquares vector with all the board indexes
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
			m_FreeSquares.push_back(std::make_pair(row, col));
	}

	// And then create 2 random squares
	for (int i = 0; i < 2; i++)
		CreateRandomNumberSquare();
}

void GameLogic::Update(const float)
{
}

void GameLogic::CreateRandomNumberSquare()
{
	int randomNumber = 2;
	if (rand() % 3 + 1 == 3) // One third chance it's a 4 instead of a 2
		randomNumber = 4;

	const auto randomIdx = m_FreeSquares[rand() % m_FreeSquares.size()];
	m_FreeSquares.erase(std::remove(m_FreeSquares.begin(), m_FreeSquares.end(), randomIdx), m_FreeSquares.end());

	const auto newNumberSquareGO = MakeNumberSquare(randomNumber, randomIdx.first, randomIdx.second);
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(newNumberSquareGO);
	m_NumberSquaresVector->push_back(newNumberSquareGO);
}

void GameLogic::RestartGame()
{
	const auto totalSquares = int(m_NumberSquaresVector->size());
	for (auto i = 0; i < totalSquares; i++)
	{
		auto numberSquare = m_NumberSquaresVector->operator[](0);
		m_NumberSquaresVector->erase(m_NumberSquaresVector->begin());
		numberSquare->GetComponent<NumberSquare>()->GetDestroyed();
	}
	
	m_FreeSquares.clear();
	m_Score = 0;
	Initialize();
}

void GameLogic::SwipeLeft()
{
	std::cout << "Left\n";
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}

void GameLogic::SwipeRight()
{
	std::cout << "Right\n";
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}

void GameLogic::SwipeUp()
{
	std::cout << "Up\n";
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}

void GameLogic::SwipeDown()
{
	std::cout << "Down\n";
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}











