#pragma once
#include <memory>
#include "BaseComponent.h"


class GameLogic final : public dae::BaseComponent
{
public:
	explicit GameLogic(const std::shared_ptr<dae::GameObject>& gameObject, float boardStartX, float boardStartY, float squareSpacing);

	GameLogic(const GameLogic& other) = delete;
	GameLogic(GameLogic&& other) noexcept = delete;
	GameLogic& operator=(const GameLogic& other) = delete;
	GameLogic& operator=(GameLogic&& other) noexcept = delete;

	~GameLogic() override = default;

	void Initialize() override;
	void Update(const float deltaTime) override;
	
	void CreateRandomNumberSquare();
	void RestartGame();

	void SwipeLeft();
	void SwipeRight();
	void SwipeUp();
	void SwipeDown();

private:
	std::shared_ptr<dae::GameObject> m_GameObject{};
	std::vector<std::shared_ptr<dae::GameObject>>* m_NumberSquaresVector{};
	std::vector<std::pair<int, int>> m_FreeSquares{};
	int m_Score;
	const float m_BoardStartX, m_BoardStartY, m_SquareSpacing;
};

