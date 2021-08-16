#pragma once
#include <memory>
#include <vector>

class GameLogic;
namespace dae
{
	class GraphicsComponent;
	class GameObject;
}

std::shared_ptr<dae::GameObject> MakeGameLogic();

std::shared_ptr<dae::GameObject> MakeBackground();

std::shared_ptr<dae::GameObject> MakeScoreDisplay(std::shared_ptr<dae::GameObject> gameLogic);

std::shared_ptr<dae::GameObject> MakeNumberSquare(int number, int rowIdx, int colIdx);

std::shared_ptr<dae::GameObject> MakeFPSCounter();

void SetUpPlayerInput(std::shared_ptr<dae::GameObject> gameLogic);