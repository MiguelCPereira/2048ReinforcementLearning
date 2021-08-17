#pragma once
#include <memory>
#include "BaseComponent.h"

class NumberSquare final : public dae::BaseComponent
{
public:
	explicit NumberSquare(const std::shared_ptr<dae::GameObject>& gameObject, int number, int rowIdx, int colIdx);
	~NumberSquare() = default;
	void Update(const float deltaTime) override;

	NumberSquare(const NumberSquare& other) = delete;
	NumberSquare(NumberSquare&& other) noexcept = delete;
	NumberSquare& operator=(const NumberSquare& other) = delete;
	NumberSquare& operator=(NumberSquare&& other) noexcept = delete;

	void GetDestroyed() const;
	
	int GetNumber() const;
	int GetRowPosIdx() const;
	int GetColPosIdx() const;
	void SetNumberBuffer(int newNumber);
	void SetRowPosIdxBuffer(int newRowPos);
	void SetColPosIdxBuffer(int newColPos);
	void UpdateValues();

private:
	std::shared_ptr<dae::GameObject> m_GameObject;
	int m_Number, m_NumberBuffer;
	int m_RowPosIdx, m_RowPosIdxBuffer;
	int m_ColPosIdx, m_ColPosIdxBuffer;
};

