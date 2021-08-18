#include "GameLogic.h"
#include <algorithm>
#include <iostream>
#include "Factory.h"
#include "GameObject.h"
#include "NumberSquare.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextComponent.h"

GameLogic::GameLogic(const std::shared_ptr<dae::GameObject>& gameObject, float squareSpacing)
	: m_GameObject(gameObject)
	, m_NumberSquaresVector()
	, m_FreeSquares()
	, m_Score()
	, m_SquareSpacing(squareSpacing)
	, m_GameOver(false)
	, m_GameOverTime(2.f)
	, m_GameOverCounter()
	, m_GameOverTitle()
{
	Initialize();
}

void GameLogic::Initialize()
{
	// Create the game over title and hide it, if it's not a restart
	if (m_GameOverTitle == nullptr)
	{
		auto gameOverTitleGO = MakeGameOverTitle();
		m_GameOverTitle = gameOverTitleGO->GetComponent<dae::TextComponent>();
		m_GameOverTitle->SetHidden(true);
		dae::SceneManager::GetInstance().GetCurrentScene()->Add(gameOverTitleGO);
	}
	
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
		CreateNumberSquare();
}

void GameLogic::Update(const float deltaTime)
{
	if(m_GameOver)
	{
		if (m_GameOverTitle->GetHidden() == true)
			m_GameOverTitle->SetHidden(false);
		
		m_GameOverCounter += deltaTime;
		if(m_GameOverCounter > m_GameOverTime)
		{
			m_GameOverTitle->SetHidden(true);
			m_GameOver = false;
			m_GameOverCounter = 0.f;
			RestartGame();
		}
	}
}

void GameLogic::CreateNumberSquare(int value, int rowIdx, int colIdx)
{
	int newSquareValue;
	std::pair<int,int> newSquareIdx;
	
	if (value == 0)
	{
		newSquareValue = 2;
		if (rand() % 3 + 1 == 3) // One third chance it's a 4 instead of a 2
			newSquareValue = 4;

		newSquareIdx = m_FreeSquares[rand() % m_FreeSquares.size()];
	}
	else
	{
		newSquareValue = value;
		newSquareIdx.first = rowIdx;
		newSquareIdx.second = colIdx;
	}

	m_FreeSquares.erase(std::remove(m_FreeSquares.begin(), m_FreeSquares.end(), newSquareIdx), m_FreeSquares.end());
	const auto newNumberSquareGO = MakeNumberSquare(newSquareValue, newSquareIdx.first, newSquareIdx.second);
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
	if (m_GameOver == false)
	{
		bool piecesMoved = false;
		std::vector<std::pair<int, int>> occupiedSquares;

		// First, organize the vector so it calculates the squares collisions left to right (considering it's gonna be a regressive for-loop)
		std::sort(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), [](std::shared_ptr<dae::GameObject>& a, std::shared_ptr<dae::GameObject>& b)
			{
				return  a->GetComponent<NumberSquare>()->GetColPosIdx() > b->GetComponent<NumberSquare>()->GetColPosIdx();
			});

		// And then collapse all colliding squares with equal value
		auto totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// If the movement will cause a collision, save the colliding square
			std::shared_ptr<dae::GameObject> collisionSquare;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetRowPosIdx() == numSquareRow && otherSquareComp->GetColPosIdx() < numSquareCol)
				{
					if (collisionSquare != nullptr)
					{
						const auto otherSquareColDist = numSquareCol - otherSquareComp->GetColPosIdx();
						const auto currentColSquareDist = numSquareCol - collisionSquare->GetComponent<NumberSquare>()->GetColPosIdx();
						if (otherSquareColDist < currentColSquareDist)
							collisionSquare = otherSquare;
					}
					else collisionSquare = otherSquare;
				}
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

			// If a colliding square has been set
			if (collisionSquare != nullptr)
			{
				auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();

				// If the colliding pieces have the same value
				if (collisionSquareComp->GetValue() == numSquareValue)
				{
					// And if none of them has collapsed already in this swipe
					if (collisionSquareComp->GetHasJustCollapsed() == false)
					{

						// Destroy the one currently being checked and double the value of the other one
						currentNumSquareComp->GetDestroyed();
						totalNumberSquares--;
						m_NumberSquaresVector->erase(std::remove(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), currentNumSquare), m_NumberSquaresVector->end());
						const auto newValue = collisionSquareComp->GetValue() * 2;
						collisionSquareComp->SetValueBuffer(newValue);
						collisionSquareComp->UpdateValues();
						m_FreeSquares.push_back(freedSpace);
						m_Score += newValue;
						m_Subject->Notify(dae::Event::ScoreIncreased);
						// Also, update the bool
						piecesMoved = true;
					}
				}
			}
		}

		// And then move everything
		totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// Calculate to which column will it move, by taking into account the other number squares in the way
			int nrSquaresInTheWay = 0;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetRowPosIdx() == numSquareRow && otherSquareComp->GetColPosIdx() < numSquareCol)
					nrSquaresInTheWay++;
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);
			auto occupiedSpace = std::make_pair(numSquareRow, nrSquaresInTheWay);

			// If the square moves
			if (occupiedSpace != freedSpace)
			{
				// Update the bool
				piecesMoved = true;

				// Save the newly occupied space and add the freed square to the vector
				m_FreeSquares.push_back(freedSpace);
				occupiedSquares.push_back(occupiedSpace);

				// Move the square, changing its position index
				currentNumSquareComp->SetColPosIdxBuffer(nrSquaresInTheWay);
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


		// If anything moved, create a new random square
		if (piecesMoved)
			CreateNumberSquare();

		// If, with the new piece, the board is now full
		if (m_FreeSquares.empty())
		{
			// Test if any future move can clear a space on the board
			if (TestSwipeLeft() == false)
			{
				if (TestSwipeRight() == false)
				{
					if (TestSwipeDown() == false)
					{
						if (TestSwipeUp() == false) // And if not, restart the game
							m_GameOver = true;
					}
				}
			}
		}
	}
}

void GameLogic::SwipeRight()
{
	if (m_GameOver == false)
	{
		bool piecesMoved = false;
		std::vector<std::pair<int, int>> occupiedSquares;

		// First, organize the vector so it calculates the squares collisions right to left (considering it's gonna be a regressive for-loop)
		std::sort(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), [](std::shared_ptr<dae::GameObject>& a, std::shared_ptr<dae::GameObject>& b)
			{
				return  a->GetComponent<NumberSquare>()->GetColPosIdx() < b->GetComponent<NumberSquare>()->GetColPosIdx();
			});


		// And then collapse all colliding squares with equal value
		auto totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// If the movement will cause a collision, save the colliding square
			std::shared_ptr<dae::GameObject> collisionSquare;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetRowPosIdx() == numSquareRow && otherSquareComp->GetColPosIdx() > numSquareCol)
				{
					if (collisionSquare != nullptr)
					{
						const auto otherSquareColDist = otherSquareComp->GetColPosIdx() - numSquareCol;
						const auto currentColSquareDist = collisionSquare->GetComponent<NumberSquare>()->GetColPosIdx() - numSquareCol;
						if (otherSquareColDist < currentColSquareDist)
							collisionSquare = otherSquare;
					}
					else collisionSquare = otherSquare;
				}
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

			// If a colliding square has been set
			if (collisionSquare != nullptr)
			{
				auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();

				// If the colliding pieces have the same value
				if (collisionSquareComp->GetValue() == numSquareValue)
				{
					// And if none of them has collapsed already in this swipe
					if (collisionSquareComp->GetHasJustCollapsed() == false)
					{
						// Destroy the one currently being checked and double the value of the other one
						currentNumSquareComp->GetDestroyed();
						totalNumberSquares--;
						m_NumberSquaresVector->erase(std::remove(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), currentNumSquare), m_NumberSquaresVector->end());
						const auto newValue = collisionSquareComp->GetValue() * 2;
						collisionSquareComp->SetValueBuffer(newValue);
						collisionSquareComp->UpdateValues();
						m_FreeSquares.push_back(freedSpace);
						m_Score += newValue;
						m_Subject->Notify(dae::Event::ScoreIncreased);
						// Also, update the bool
						piecesMoved = true;
					}
				}
			}
		}

		// And then move everything
		totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// Calculate to which column will it move, by taking into account the other number squares in the way
			int nrSquaresInTheWay = 0;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetRowPosIdx() == numSquareRow && otherSquareComp->GetColPosIdx() > numSquareCol)
					nrSquaresInTheWay++;
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);
			auto occupiedSpace = std::make_pair(numSquareRow, 3 - nrSquaresInTheWay);

			// If the square moves
			if (occupiedSpace != freedSpace)
			{
				// Update the bool
				piecesMoved = true;

				// Save the newly occupied space and add the freed square to the vector
				m_FreeSquares.push_back(freedSpace);
				occupiedSquares.push_back(occupiedSpace);

				// Move the square, changing its position index
				currentNumSquareComp->SetColPosIdxBuffer(3 - nrSquaresInTheWay);
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


		// If anything moved, create a new random square
		if (piecesMoved)
			CreateNumberSquare();

		// If, with the new piece, the board is now full
		if (m_FreeSquares.empty())
		{
			// Test if any future move can clear a space on the board
			if (TestSwipeLeft() == false)
			{
				if (TestSwipeRight() == false)
				{
					if (TestSwipeDown() == false)
					{
						if (TestSwipeUp() == false) // And if not, restart the game
							m_GameOver = true;
					}
				}
			}
		}
	}
}

void GameLogic::SwipeUp()
{
	if (m_GameOver == false)
	{
		bool piecesMoved = false;
		std::vector<std::pair<int, int>> occupiedSquares;

		// First, organize the vector so it calculates the squares collisions top to bottom (considering it's gonna be a regressive for-loop)
		std::sort(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), [](std::shared_ptr<dae::GameObject>& a, std::shared_ptr<dae::GameObject>& b)
			{
				return  a->GetComponent<NumberSquare>()->GetRowPosIdx() > b->GetComponent<NumberSquare>()->GetRowPosIdx();
			});

		// And then collapse all colliding squares with equal value
		auto totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// If the movement will cause a collision, save the colliding square
			std::shared_ptr<dae::GameObject> collisionSquare;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetColPosIdx() == numSquareCol && otherSquareComp->GetRowPosIdx() < numSquareRow)
				{
					if (collisionSquare != nullptr)
					{
						const auto otherSquareRowDist = numSquareRow - otherSquareComp->GetRowPosIdx();
						const auto currentRowSquareDist = numSquareRow - collisionSquare->GetComponent<NumberSquare>()->GetRowPosIdx();
						if (otherSquareRowDist < currentRowSquareDist)
							collisionSquare = otherSquare;
					}
					else collisionSquare = otherSquare;
				}
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

			// If a colliding square has been set
			if (collisionSquare != nullptr)
			{
				auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();

				// If the colliding pieces have the same value
				if (collisionSquareComp->GetValue() == numSquareValue)
				{
					// And if none of them has collapsed already in this swipe
					if (collisionSquareComp->GetHasJustCollapsed() == false)
					{
						// Destroy the one currently being checked and double the value of the other one
						currentNumSquareComp->GetDestroyed();
						totalNumberSquares--;
						m_NumberSquaresVector->erase(std::remove(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), currentNumSquare), m_NumberSquaresVector->end());
						const auto newValue = collisionSquareComp->GetValue() * 2;
						collisionSquareComp->SetValueBuffer(newValue);
						collisionSquareComp->UpdateValues();
						m_FreeSquares.push_back(freedSpace);
						m_Score += newValue;
						m_Subject->Notify(dae::Event::ScoreIncreased);
						// Also, update the bool
						piecesMoved = true;
					}
				}
			}
		}

		// And then move everything
		totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// Calculate to which column will it move, by taking into account the other number squares in the way
			int nrSquaresInTheWay = 0;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetColPosIdx() == numSquareCol && otherSquareComp->GetRowPosIdx() < numSquareRow)
					nrSquaresInTheWay++;
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);
			auto occupiedSpace = std::make_pair(nrSquaresInTheWay, numSquareCol);

			// If the square moves
			if (occupiedSpace != freedSpace)
			{
				// Update the bool
				piecesMoved = true;

				// Save the newly occupied space and add the freed square to the vector
				m_FreeSquares.push_back(freedSpace);
				occupiedSquares.push_back(occupiedSpace);

				// Move the square, changing its position index
				currentNumSquareComp->SetRowPosIdxBuffer(nrSquaresInTheWay);
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


		// If anything moved, create a new random square
		if (piecesMoved)
			CreateNumberSquare();

		// If, with the new piece, the board is now full
		if (m_FreeSquares.empty())
		{
			// Test if any future move can clear a space on the board
			if (TestSwipeLeft() == false)
			{
				if (TestSwipeRight() == false)
				{
					if (TestSwipeDown() == false)
					{
						if (TestSwipeUp() == false) // And if not, restart the game
							m_GameOver = true;
					}
				}
			}
		}
	}
}

void GameLogic::SwipeDown()
{
	if (m_GameOver == false)
	{
		bool piecesMoved = false;
		std::vector<std::pair<int, int>> occupiedSquares;

		// First, organize the vector so it calculates the squares collisions bottom to top (considering it's gonna be a regressive for-loop)
		std::sort(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), [](std::shared_ptr<dae::GameObject>& a, std::shared_ptr<dae::GameObject>& b)
			{
				return  a->GetComponent<NumberSquare>()->GetRowPosIdx() < b->GetComponent<NumberSquare>()->GetRowPosIdx();
			});

		// And then collapse all colliding squares with equal value
		auto totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// If the movement will cause a collision, save the colliding square
			std::shared_ptr<dae::GameObject> collisionSquare;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetColPosIdx() == numSquareCol && otherSquareComp->GetRowPosIdx() > numSquareRow)
				{
					if (collisionSquare != nullptr)
					{
						const auto otherSquareRowDist = otherSquareComp->GetRowPosIdx() - numSquareRow;
						const auto currentRowSquareDist = collisionSquare->GetComponent<NumberSquare>()->GetRowPosIdx() - numSquareRow;
						if (otherSquareRowDist < currentRowSquareDist)
							collisionSquare = otherSquare;
					}
					else collisionSquare = otherSquare;
				}
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);

			// If a colliding square has been set
			if (collisionSquare != nullptr)
			{
				auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();

				// If the colliding pieces have the same value
				if (collisionSquareComp->GetValue() == numSquareValue)
				{
					// And if none of them has collapsed already in this swipe
					if (collisionSquareComp->GetHasJustCollapsed() == false)
					{
						// Destroy the one currently being checked and double the value of the other one
						currentNumSquareComp->GetDestroyed();
						totalNumberSquares--;
						m_NumberSquaresVector->erase(std::remove(m_NumberSquaresVector->begin(), m_NumberSquaresVector->end(), currentNumSquare), m_NumberSquaresVector->end());
						const auto newValue = collisionSquareComp->GetValue() * 2;
						collisionSquareComp->SetValueBuffer(newValue);
						collisionSquareComp->UpdateValues();
						m_FreeSquares.push_back(freedSpace);
						m_Score += newValue;
						m_Subject->Notify(dae::Event::ScoreIncreased);
						// Also, update the bool
						piecesMoved = true;
					}
				}
			}
		}

		// And then move everything
		totalNumberSquares = m_NumberSquaresVector->size();
		for (auto i = totalNumberSquares; i > 0; i--)
		{
			// Get all the needed info from the current number square
			const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
			auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
			auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
			auto numSquareCol = currentNumSquareComp->GetColPosIdx();
			const auto numSquareValue = currentNumSquareComp->GetValue();

			// Calculate to which column will it move, by taking into account the other number squares in the way
			int nrSquaresInTheWay = 0;
			for (auto e = totalNumberSquares; e > 0; e--)
			{
				auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
				const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
				if (otherSquareComp->GetColPosIdx() == numSquareCol && otherSquareComp->GetRowPosIdx() > numSquareRow)
					nrSquaresInTheWay++;
			}

			auto freedSpace = std::make_pair(numSquareRow, numSquareCol);
			auto occupiedSpace = std::make_pair(3 - nrSquaresInTheWay, numSquareCol);

			// If the square moves
			if (occupiedSpace != freedSpace)
			{
				// Update the bool
				piecesMoved = true;

				// Save the newly occupied space and add the freed square to the vector
				m_FreeSquares.push_back(freedSpace);
				occupiedSquares.push_back(occupiedSpace);

				// Move the square, changing its position index
				currentNumSquareComp->SetRowPosIdxBuffer(3 - nrSquaresInTheWay);
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


		// If anything moved, create a new random square
		if (piecesMoved)
			CreateNumberSquare();

		// If, with the new piece, the board is now full
		if (m_FreeSquares.empty())
		{
			// Test if any future move can clear a space on the board
			if (TestSwipeLeft() == false)
			{
				if (TestSwipeRight() == false)
				{
					if (TestSwipeDown() == false)
					{
						if (TestSwipeUp() == false) // And if not, restart the game
							m_GameOver = true;
					}
				}
			}
		}
	}
}



bool GameLogic::TestSwipeLeft() const
{
	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
		const auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
		const auto numSquareCol = currentNumSquareComp->GetColPosIdx();
		const auto numSquareValue = currentNumSquareComp->GetValue();

		// Save the square with which the current one will collide
		std::shared_ptr<dae::GameObject> collisionSquare;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
			const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
			if (otherSquareComp->GetRowPosIdx() == numSquareRow && otherSquareComp->GetColPosIdx() < numSquareCol)
			{
				if (collisionSquare != nullptr)
				{
					const auto otherSquareColDist = numSquareCol - otherSquareComp->GetColPosIdx();
					const auto currentColSquareDist = numSquareCol - collisionSquare->GetComponent<NumberSquare>()->GetColPosIdx();
					if (otherSquareColDist < currentColSquareDist)
						collisionSquare = otherSquare;
				}
				else collisionSquare = otherSquare;
			}
		}

		// If there's a collision
		if (collisionSquare != nullptr)
		{
			// If the colliding pieces have the same value
			auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();
			if (collisionSquareComp->GetValue() == numSquareValue)
				return true;
		}
	}

	return false;
}

bool GameLogic::TestSwipeRight() const
{
	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
		const auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
		const auto numSquareCol = currentNumSquareComp->GetColPosIdx();
		const auto numSquareValue = currentNumSquareComp->GetValue();

		// Save the square with which the current one will collide
		std::shared_ptr<dae::GameObject> collisionSquare;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
			const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
			if (otherSquareComp->GetRowPosIdx() == numSquareRow && otherSquareComp->GetColPosIdx() > numSquareCol)
			{
				if (collisionSquare != nullptr)
				{
					const auto otherSquareColDist = otherSquareComp->GetColPosIdx() - numSquareCol;
					const auto currentColSquareDist = collisionSquare->GetComponent<NumberSquare>()->GetColPosIdx() - numSquareCol;
					if (otherSquareColDist < currentColSquareDist)
						collisionSquare = otherSquare;
				}
				else collisionSquare = otherSquare;
			}
		}

		// If there's a collision
		if (collisionSquare != nullptr)
		{
			// If the colliding pieces have the same value
			auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();
			if (collisionSquareComp->GetValue() == numSquareValue)
				return true;
		}
	}

	return false;
}

bool GameLogic::TestSwipeUp() const
{
	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
		const auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
		const auto numSquareCol = currentNumSquareComp->GetColPosIdx();
		const auto numSquareValue = currentNumSquareComp->GetValue();

		// Save the square with which the current one will collide
		std::shared_ptr<dae::GameObject> collisionSquare;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
			const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
			if (otherSquareComp->GetColPosIdx() == numSquareCol && otherSquareComp->GetRowPosIdx() < numSquareRow)
			{
				if (collisionSquare != nullptr)
				{
					const auto otherSquareRowDist = numSquareRow - otherSquareComp->GetRowPosIdx();
					const auto currentRowSquareDist = numSquareRow - collisionSquare->GetComponent<NumberSquare>()->GetRowPosIdx();
					if (otherSquareRowDist < currentRowSquareDist)
						collisionSquare = otherSquare;
				}
				else collisionSquare = otherSquare;
			}
		}

		// If there's a collision
		if (collisionSquare != nullptr)
		{
			// If the colliding pieces have the same value
			auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();
			if (collisionSquareComp->GetValue() == numSquareValue)
				return true;
		}
	}

	return false;
}

bool GameLogic::TestSwipeDown() const
{
	const auto totalNumberSquares = m_NumberSquaresVector->size();
	for (auto i = totalNumberSquares; i > 0; i--)
	{
		// Get all the needed info from the current number square
		const auto& currentNumSquare = m_NumberSquaresVector->operator[](i - 1);
		auto* currentNumSquareComp = currentNumSquare->GetComponent<NumberSquare>();
		const auto numSquareRow = currentNumSquareComp->GetRowPosIdx();
		const auto numSquareCol = currentNumSquareComp->GetColPosIdx();
		const auto numSquareValue = currentNumSquareComp->GetValue();

		// Save the square with which the current one will collide
		std::shared_ptr<dae::GameObject> collisionSquare;
		for (auto e = totalNumberSquares; e > 0; e--)
		{
			auto otherSquare = m_NumberSquaresVector->operator[](e - 1);
			const auto* otherSquareComp = otherSquare->GetComponent<NumberSquare>();
			if (otherSquareComp->GetColPosIdx() == numSquareCol && otherSquareComp->GetRowPosIdx() > numSquareRow)
			{
				if (collisionSquare != nullptr)
				{
					const auto otherSquareRowDist = otherSquareComp->GetRowPosIdx() - numSquareRow;
					const auto currentRowSquareDist = collisionSquare->GetComponent<NumberSquare>()->GetRowPosIdx() - numSquareRow;
					if (otherSquareRowDist < currentRowSquareDist)
						collisionSquare = otherSquare;
				}
				else collisionSquare = otherSquare;
			}
		}

		// If there's a collision
		if (collisionSquare != nullptr)
		{
			// If the colliding pieces have the same value
			auto* collisionSquareComp = collisionSquare->GetComponent<NumberSquare>();
			if (collisionSquareComp->GetValue() == numSquareValue)
				return true;
		}
	}

	return false;
}













