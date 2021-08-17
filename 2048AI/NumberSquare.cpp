#include "NumberSquare.h"
#include "GameObject.h"

NumberSquare::NumberSquare(const std::shared_ptr<dae::GameObject>& gameObject, int number, int rowIdx, int colIdx)
	: m_GameObject(gameObject)
	, m_Number(number)
	, m_NumberBuffer(number)
	, m_RowPosIdx(rowIdx)
	, m_RowPosIdxBuffer(rowIdx)
	, m_ColPosIdx(colIdx)
	, m_ColPosIdxBuffer(colIdx)
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

void NumberSquare::SetNumberBuffer(int newNumber)
{
	m_NumberBuffer = newNumber;
}

void NumberSquare::SetRowPosIdxBuffer(int newRowPos)
{
	m_RowPosIdxBuffer = newRowPos;
}

void NumberSquare::SetColPosIdxBuffer(int newColPos)
{
	m_ColPosIdxBuffer = newColPos;
}

void NumberSquare::UpdateValues()
{
	m_RowPosIdx = m_RowPosIdxBuffer;
	m_ColPosIdx  = m_ColPosIdxBuffer;
	m_Number = m_NumberBuffer;
}
