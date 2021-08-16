#include "NumberSquare.h"

NumberSquare::NumberSquare(const std::shared_ptr<dae::GameObject>& gameObject, int number, int boardPosIdx)
	: m_GameObject(gameObject)
	, m_Number(number)
	, m_BoardPosIdx(boardPosIdx)
{
}

void NumberSquare::Update(const float)
{
}


int NumberSquare::GetNumber() const
{
	return m_Number;
}

int NumberSquare::GetBoardPosIdx() const
{
	return m_BoardPosIdx;
}

void NumberSquare::SetBoardPosIdx(int newBoardPos)
{
	m_BoardPosIdx = newBoardPos;
}
