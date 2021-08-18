#pragma once
#include <memory>
#include "BaseComponent.h"


class GameLogic final : public dae::BaseComponent
{
public:
	explicit GameLogic(const std::shared_ptr<dae::GameObject>& gameObject, float squareSpacing);

	GameLogic(const GameLogic& other) = delete;
	GameLogic(GameLogic&& other) noexcept = delete;
	GameLogic& operator=(const GameLogic& other) = delete;
	GameLogic& operator=(GameLogic&& other) noexcept = delete;

	~GameLogic() override = default;

	void Initialize() override;
	void Update(const float deltaTime) override;
	
	void CreateNumberSquare(int value = 0, int rowIdx = 0, int colIdx = 0);
	void RestartGame();

	int GetScore() const { return m_Score; }

	void SwipeLeft();
	void SwipeRight();
	void SwipeUp();
	void SwipeDown();

	bool TestSwipeLeft() const;
	bool TestSwipeRight() const;
	bool TestSwipeUp() const;
	bool TestSwipeDown() const;

private:
	std::shared_ptr<dae::GameObject> m_GameObject{};
	std::vector<std::shared_ptr<dae::GameObject>>* m_NumberSquaresVector{};
	std::vector<std::pair<int, int>> m_FreeSquares{};
	int m_Score;
	const float m_SquareSpacing;
	bool m_GameOver;
	const float m_GameOverTime;
	float m_GameOverCounter;
	dae::TextComponent* m_GameOverTitle;
};

