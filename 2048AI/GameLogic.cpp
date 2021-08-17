#include "GameLogic.h"
#include <iostream>
#include "Factory.h"
#include "GameObject.h"
#include "NumberSquare.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GraphicsComponent.h"
#include "TextComponent.h"

GameLogic::GameLogic(const std::shared_ptr<dae::GameObject>& gameObject, float boardStartX, float boardStartY, float squareSpacing)
	: m_GameObject(gameObject)
	, m_NumberSquaresVector()
	, m_FreeSquares()
	, m_Score()
	, m_BoardStartX(boardStartX)
	, m_BoardStartY(boardStartY)
	, m_SquareSpacing(squareSpacing)
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
	std::vector<std::pair<int, int>> occupiedSquares;

	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for(auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto numSquareRow = currentNumSquare->GetComponent<NumberSquare>()->GetRowPosIdx();
		auto numSquareCol = currentNumSquare->GetComponent<NumberSquare>()->GetColPosIdx();

		// Calculate to which column will it move, by taking into account the other number squares in the way
		int nrSquaresInTheWay = 0;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			const auto& otherSquare = m_NumberSquaresVector->operator[](e - 1)->GetComponent<NumberSquare>();
			if (otherSquare->GetRowPosIdx() == numSquareRow && otherSquare->GetColPosIdx() < numSquareCol)
				nrSquaresInTheWay++;
		}

		auto occupiedSpace = std::make_pair(numSquareRow, nrSquaresInTheWay);
		auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

		// If the square moves
		if (occupiedSpace != freedSpace)
		{
			// Save the newly occupied space and add the freed square to the vector
			m_FreeSquares.push_back(freedSpace);
			occupiedSquares.push_back(occupiedSpace);

			// Move the square, changing both its graphics and its position indexes
			currentNumSquare->GetComponent<NumberSquare>()->SetColPosIdxBuffer(nrSquaresInTheWay);

			auto* graphicsComponent = currentNumSquare->GetComponent<dae::GraphicsComponent>();
			graphicsComponent->SetPosition(graphicsComponent->GetPosition().first - m_SquareSpacing * float(numSquareCol - nrSquaresInTheWay),
				graphicsComponent->GetPosition().second);

			const auto* textComponent = currentNumSquare->GetComponent<dae::TextComponent>();
			textComponent->SetPosition(textComponent->GetPosition().first - m_SquareSpacing * float(numSquareCol - nrSquaresInTheWay),
				textComponent->GetPosition().second);
		}
	}

	// Actually change the number-squares indexes, after all their movements are calculated
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		currentNumSquare->GetComponent<NumberSquare>()->UpdateValues();
	}
	
	// Update the m_FreeSquares vector with all the newly occupied spaces
	for (auto newOccupiedSquare : occupiedSquares)
		m_FreeSquares.erase(std::remove(m_FreeSquares.begin(), m_FreeSquares.end(), newOccupiedSquare), m_FreeSquares.end());

	// Create a new random square or restart the game if the board is full
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}

void GameLogic::SwipeRight()
{
	std::cout << "Right\n";
	std::vector<std::pair<int, int>> occupiedSquares;
	
	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto numSquareRow = currentNumSquare->GetComponent<NumberSquare>()->GetRowPosIdx();
		auto numSquareCol = currentNumSquare->GetComponent<NumberSquare>()->GetColPosIdx();

		// Calculate to which column will it move, by taking into account the other number squares in the way
		int nrSquaresInTheWay = 0;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			const auto& otherSquare = m_NumberSquaresVector->operator[](e - 1)->GetComponent<NumberSquare>();
			if (otherSquare->GetRowPosIdx() == numSquareRow && otherSquare->GetColPosIdx() > numSquareCol)
				nrSquaresInTheWay++;
		}

		auto occupiedSpace = std::make_pair(numSquareRow, 3 - nrSquaresInTheWay);
		auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

		// If the square moves
		if (occupiedSpace != freedSpace)
		{
			// Save the newly occupied space and add the freed square to the vector
			m_FreeSquares.push_back(freedSpace);
			occupiedSquares.push_back(occupiedSpace);

			// Move the square, changing both its graphics and its position indexes
			currentNumSquare->GetComponent<NumberSquare>()->SetColPosIdxBuffer(3 - nrSquaresInTheWay);

			auto* graphicsComponent = currentNumSquare->GetComponent<dae::GraphicsComponent>();
			graphicsComponent->SetPosition(graphicsComponent->GetPosition().first + m_SquareSpacing * float(3 - nrSquaresInTheWay - numSquareCol),
				graphicsComponent->GetPosition().second);

			const auto* textComponent = currentNumSquare->GetComponent<dae::TextComponent>();
			textComponent->SetPosition(textComponent->GetPosition().first + m_SquareSpacing * float(3 - nrSquaresInTheWay - numSquareCol),
				textComponent->GetPosition().second);
		}
	}

	// Actually change the number-squares indexes, after all their movements are calculated
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		currentNumSquare->GetComponent<NumberSquare>()->UpdateValues();
	}

	// Update the m_FreeSquares vector with all the newly occupied spaces
	for(auto newOccupiedSquare : occupiedSquares)
		m_FreeSquares.erase(std::remove(m_FreeSquares.begin(), m_FreeSquares.end(), newOccupiedSquare), m_FreeSquares.end());

	// Create a new random square or restart the game if the board is full
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}

void GameLogic::SwipeUp()
{
	std::cout << "Up\n";
	std::vector<std::pair<int, int>> occupiedSquares;

	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto numSquareRow = currentNumSquare->GetComponent<NumberSquare>()->GetRowPosIdx();
		auto numSquareCol = currentNumSquare->GetComponent<NumberSquare>()->GetColPosIdx();

		// Calculate to which column will it move, by taking into account the other number squares in the way
		int nrSquaresInTheWay = 0;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			const auto& otherSquare = m_NumberSquaresVector->operator[](e - 1)->GetComponent<NumberSquare>();
			if (otherSquare->GetColPosIdx() == numSquareCol && otherSquare->GetRowPosIdx() < numSquareRow)
				nrSquaresInTheWay++;
		}

		auto occupiedSpace = std::make_pair(nrSquaresInTheWay, numSquareCol);
		auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

		// If the square moves
		if (occupiedSpace != freedSpace)
		{
			// Save the newly occupied space and add the freed square to the vector
			m_FreeSquares.push_back(freedSpace);
			occupiedSquares.push_back(occupiedSpace);

			// Move the square, changing both its graphics and its position indexes
			currentNumSquare->GetComponent<NumberSquare>()->SetRowPosIdxBuffer(nrSquaresInTheWay);

			auto* graphicsComponent = currentNumSquare->GetComponent<dae::GraphicsComponent>();
			graphicsComponent->SetPosition(graphicsComponent->GetPosition().first,
				graphicsComponent->GetPosition().second - m_SquareSpacing * float(numSquareRow - nrSquaresInTheWay));

			const auto* textComponent = currentNumSquare->GetComponent<dae::TextComponent>();
			textComponent->SetPosition(textComponent->GetPosition().first,
				textComponent->GetPosition().second - m_SquareSpacing * float(numSquareRow - nrSquaresInTheWay));
		}
	}

	// Actually change the number-squares indexes, after all their movements are calculated
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		currentNumSquare->GetComponent<NumberSquare>()->UpdateValues();
	}

	// Update the m_FreeSquares vector with all the newly occupied spaces
	for (auto newOccupiedSquare : occupiedSquares)
		m_FreeSquares.erase(std::remove(m_FreeSquares.begin(), m_FreeSquares.end(), newOccupiedSquare), m_FreeSquares.end());

	// Create a new random square or restart the game if the board is full
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}

void GameLogic::SwipeDown()
{
	std::cout << "Down\n";
	std::vector<std::pair<int, int>> occupiedSquares;

	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto numSquareRow = currentNumSquare->GetComponent<NumberSquare>()->GetRowPosIdx();
		auto numSquareCol = currentNumSquare->GetComponent<NumberSquare>()->GetColPosIdx();

		// Calculate to which column will it move, by taking into account the other number squares in the way
		int nrSquaresInTheWay = 0;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			const auto& otherSquare = m_NumberSquaresVector->operator[](e - 1)->GetComponent<NumberSquare>();
			if (otherSquare->GetColPosIdx() == numSquareCol && otherSquare->GetRowPosIdx() > numSquareRow)
				nrSquaresInTheWay++;
		}

		auto occupiedSpace = std::make_pair(3 - nrSquaresInTheWay, numSquareCol);
		auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

		// If the square moves
		if (occupiedSpace != freedSpace)
		{
			// Save the newly occupied space and add the freed square to the vector
			m_FreeSquares.push_back(freedSpace);
			occupiedSquares.push_back(occupiedSpace);

			// Move the square, changing both its graphics and its position indexes
			currentNumSquare->GetComponent<NumberSquare>()->SetRowPosIdxBuffer(3 - nrSquaresInTheWay);

			auto* graphicsComponent = currentNumSquare->GetComponent<dae::GraphicsComponent>();
			graphicsComponent->SetPosition(graphicsComponent->GetPosition().first,
				graphicsComponent->GetPosition().second + m_SquareSpacing * float(3 - nrSquaresInTheWay - numSquareRow));

			const auto* textComponent = currentNumSquare->GetComponent<dae::TextComponent>();
			textComponent->SetPosition(textComponent->GetPosition().first,
				textComponent->GetPosition().second + m_SquareSpacing * float(3 - nrSquaresInTheWay - numSquareRow));
		}
	}

	// Actually change the number-squares indexes, after all their movements are calculated
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		currentNumSquare->GetComponent<NumberSquare>()->UpdateValues();
	}

	// Update the m_FreeSquares vector with all the newly occupied spaces
	for (auto newOccupiedSquare : occupiedSquares)
		m_FreeSquares.erase(std::remove(m_FreeSquares.begin(), m_FreeSquares.end(), newOccupiedSquare), m_FreeSquares.end());

	// Create a new random square or restart the game if the board is full
	if (m_FreeSquares.empty() == false)
		CreateRandomNumberSquare();
	else
		RestartGame();
}











