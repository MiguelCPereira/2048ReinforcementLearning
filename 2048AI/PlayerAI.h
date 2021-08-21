#pragma once
#include <deque>
#include <memory>
#include "BaseComponent.h"
#include "QLearningTrainer.h"

class GameLogic;
class LinearQNet;

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
	int CalculateAction(std::vector<int>* oldState) const;
	void Remember(TrainingInfo* trainingInfo);
	void TrainStepWithSample();
	void Train();
	
	std::shared_ptr<dae::GameObject> m_GameObject{};
	GameLogic* m_GameLogic{};
	const float m_TimeBetweenMoves;
	float m_MoveTimeCounter;
	std::vector<int> m_AllScores;
	int m_Highscore;
	int m_NrPlayedGames;

	const double m_LearningRate;
	const int m_RandomFactor;
	float m_Discount;
	const size_t m_MaxMemory;
	const size_t m_TrainingBatchSize;
	std::deque<TrainingInfo*> m_Memory;
	LinearQNet* m_Model;
	QLearningTrainer* m_Trainer;
};

