#include "NumberSquare.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "TextComponent.h"

NumberSquare::NumberSquare(const std::shared_ptr<dae::GameObject>& gameObject, int number, int rowIdx, int colIdx, float spaceBetweenSquares, bool withVisuals)
	: m_GameObject(gameObject)
	, m_Value(number)
	, m_ValueBuffer(number)
	, m_RowPosIdx(rowIdx)
	, m_RowPosIdxBuffer(rowIdx)
	, m_ColPosIdx(colIdx)
	, m_ColPosIdxBuffer(colIdx)
	, m_SquareSpacing(spaceBetweenSquares)
	, m_HasJustCollapsed(false)
	, m_WithVisuals(withVisuals)
{
}

void NumberSquare::Update(const float)
{
}

void NumberSquare::GetDestroyed() const
{
	m_GameObject->RemoveAllComponents();
}

int NumberSquare::GetValue() const
{
	return m_Value;
}

int NumberSquare::GetRowPosIdx() const
{
	return m_RowPosIdx;
}

int NumberSquare::GetColPosIdx() const
{
	return m_ColPosIdx;
}

void NumberSquare::SetValueBuffer(int newValue)
{
	m_HasJustCollapsed = true;
	m_ValueBuffer = newValue;
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
	if (m_WithVisuals)
	{
		auto* text = m_GameObject->GetComponent<dae::TextComponent>();

		if (m_RowPosIdx != m_RowPosIdxBuffer || m_ColPosIdx != m_ColPosIdxBuffer)
		{
			auto* graphics = m_GameObject->GetComponent<dae::GraphicsComponent>();
			graphics->SetPosition(graphics->GetPosition().first + m_SquareSpacing * float(m_ColPosIdxBuffer - m_ColPosIdx),
				graphics->GetPosition().second + m_SquareSpacing * float(m_RowPosIdxBuffer - m_RowPosIdx));

			text->SetPosition(text->GetPosition().first + m_SquareSpacing * float(m_ColPosIdxBuffer - m_ColPosIdx),
				text->GetPosition().second + m_SquareSpacing * float(m_RowPosIdxBuffer - m_RowPosIdx));

			m_RowPosIdx = m_RowPosIdxBuffer;
			m_ColPosIdx = m_ColPosIdxBuffer;
		}

		if (m_Value != m_ValueBuffer)
		{
			const auto newValueText = std::to_string(m_ValueBuffer);
			const auto oldValueText = std::to_string(m_Value);
			text->SetText(newValueText);

			// If the new value has 1 more digit than the previous one
			if (newValueText.size() > oldValueText.size())
			{
				// Offset the text a bit
				const auto charOffset = 6.f;
				text->SetPosition(text->GetPosition().first - charOffset, text->GetPosition().second);
			}

			m_Value = m_ValueBuffer;
			m_HasJustCollapsed = false;
		}
	}
	else
	{
		if (m_RowPosIdx != m_RowPosIdxBuffer || m_ColPosIdx != m_ColPosIdxBuffer)
		{
			m_RowPosIdx = m_RowPosIdxBuffer;
			m_ColPosIdx = m_ColPosIdxBuffer;
		}

		if (m_Value != m_ValueBuffer)
		{
			m_Value = m_ValueBuffer;
			m_HasJustCollapsed = false;
		}
	}
}
