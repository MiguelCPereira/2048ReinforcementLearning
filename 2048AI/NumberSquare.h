#pragma once
#include <memory>

#include "BaseComponent.h"

class NumberSquare final : public dae::BaseComponent
{
public:
	explicit NumberSquare(const std::shared_ptr<dae::GameObject>& gameObject, int number, int boardPosIdx);
	~NumberSquare() = default;
	void Update(const float deltaTime) override;

	NumberSquare(const NumberSquare& other) = delete;
	NumberSquare(NumberSquare&& other) noexcept = delete;
	NumberSquare& operator=(const NumberSquare& other) = delete;
	NumberSquare& operator=(NumberSquare&& other) noexcept = delete;

	int GetNumber() const;
	int GetBoardPosIdx() const;
	void SetBoardPosIdx(int newBoardPos);

private:
	std::shared_ptr<dae::GameObject> m_GameObject;
	int m_Number;
	int m_BoardPosIdx;
};

