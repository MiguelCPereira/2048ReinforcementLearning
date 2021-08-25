#pragma once
#include <memory>
#include <vector>

class GameLogic;
namespace dae
{
	class GraphicsComponent;
	class GameObject;
}

std::shared_ptr<dae::GameObject> MakeGameLogic(bool withVisuals = true);

std::shared_ptr<dae::GameObject> MakeBackground();

std::shared_ptr<dae::GameObject> MakeScoreDisplay(const std::shared_ptr<dae::GameObject>& gameLogic);

std::shared_ptr<dae::GameObject> MakeNumberSquare(int number, int rowIdx, int colIdx, bool withVisuals);

std::shared_ptr<dae::GameObject> MakeGameOverTitle();

std::shared_ptr<dae::GameObject> MakeFPSCounter();

void SetUpPlayerInput(std::shared_ptr<dae::GameObject> gameLogic);

std::shared_ptr<dae::GameObject> MakePlayerAI(std::shared_ptr<dae::GameObject> gameLogic, float timeBetweenMoves);