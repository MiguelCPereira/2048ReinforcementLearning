#include "NumberSquare.h"
#include "GameObject.h"

NumberSquare::NumberSquare(const std::shared_ptr<dae::GameObject>& gameObject, int number, int rowIdx, int colIdx)
	: m_GameObject(gameObject)
	, m_Number(number)
	, m_RowPosIdx(rowIdx)
	, m_ColPosIdx(colIdx)
{
}

void NumberSquare::Update(const float)
{
}

void NumberSquare::GetDestroyed() const
{
	m_GameObject->RemoveAllComponents();
}

int NumberSquare::GetNumber() const
{
	return m_Number;
}

int NumberSquare::GetRowPosIdx() const
{
	return m_RowPosIdx;
}

int NumberSquare::GetColPosIdx() const
{
	return m_ColPosIdx;
}

void NumberSquare::SetNumber(int newNumber)
{
	m_Number = newNumber;
}

void NumberSquare::SetRowPosIdx(int newRowPos)
{
	m_RowPosIdx = newRowPos;
}

void NumberSquare::SetColPosIdx(int newColPos)
{
	m_ColPosIdx = newColPos;
}
