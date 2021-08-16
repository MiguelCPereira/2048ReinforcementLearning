#pragma once
#include <memory>
#include <vector>

namespace dae
{
	class GraphicsComponent;
	class GameObject;
}

std::shared_ptr<dae::GameObject> MakeBackground();

std::shared_ptr<dae::GameObject> MakeScoreDisplay(/*dae::GameLogic* gameLogicComp*/);

std::shared_ptr<dae::GameObject> MakeNumberSquare(int number, int boardPosIdx);

std::shared_ptr<dae::GameObject> MakeFPSCounter();