#pragma once
#include <deque>
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


	// These functions and member variables are still unfinished and mostly just wishful planning.
	// As so, duo to their early unrefined state, the overall structure and logic behind them
	// is still very reminiscent to this Snake Python tutorial:
	// https://www.youtube.com/watch?v=PJl4iabBEz0&t=0s

	struct TrainingInfo
	{
		std::vector<int> oldState;
		std::vector<int> newState;
		int nextMove;
		int reward;
		bool gameOver;
	};
	
	int CalculateAction(const std::vector<int>& oldState) const;
	void Remember(const TrainingInfo& trainingInfo);
	void TrainShortMemory(const TrainingInfo& trainingInfo);
	void TrainLongMemory();
	void Plot();
	void Train();
	
	std::vector<int> m_PlotScores;
	std::vector<int> m_PlotMeanScores;
	int m_TotalScore = 0;
	int m_Highscore = 0;
	int m_NrPlayedGames = 0;

	const int m_RandomFactor;
	float m_Gamma; // Discount rate
	const size_t m_MaxMemory;
	const size_t m_TrainingBatchSize;
	std::deque<TrainingInfo> m_Memory;
	//Model m_Model;
	//QLearning m_Trainer;
};

