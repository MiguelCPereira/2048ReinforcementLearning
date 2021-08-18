#pragma once
#include <memory>
#include "BaseComponent.h"

class GameLogic;

class PlayerAI final : public dae::BaseComponent
{
public:
	explicit PlayerAI(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::GameObject>& gameLogicGObj, float timeBetweenMoves);

	PlayerAI(const PlayerAI& other) = delete;
	PlayerAI(PlayerAI&& other) noexcept = delete;
	PlayerAI& operator=(const PlayerAI& other) = delete;
	PlayerAI& operator=(PlayerAI&& other) noexcept = delete;

	~PlayerAI() override = default;

	void Initialize() override;
	void Update(const float deltaTime) override;

private:
	std::shared_ptr<dae::GameObject> m_GameObject{};
	GameLogic* m_GameLogic{};
	const float m_TimeBetweenMoves;
	float m_MoveTimeCounter;
};

