#include "Factory.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "ResourceManager.h"
#include "FPSComponent.h"
#include "NumberSquare.h"
#include "TextComponent.h"
//#include "ScoreDisplay.h"

float g_BoardStartX = 155.f;
float g_BoardStartY = 80.f;

std::shared_ptr<dae::GameObject> MakeBackground()
{
	const auto backgWidthHeight = 326.f;
	
	auto backgroundGO = std::make_shared<dae::GameObject>();
	backgroundGO->AddComponent(new dae::GraphicsComponent("Board.png", g_BoardStartX, g_BoardStartY, backgWidthHeight, backgWidthHeight));
	
	return backgroundGO;
}

std::shared_ptr<dae::GameObject> MakeScoreDisplay(/*dae::GameLogic* gameLogicComp*/)
{
	const auto posX = 20.f;
	const auto posY = 20.f;

	auto scoreGO = std::make_shared<dae::GameObject>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	scoreGO->AddComponent(new dae::TextComponent("SCORE: FAIL", font, 238, 228, 218));
	scoreGO->GetComponent<dae::TextComponent>()->SetPosition(posX, posY);
	//scoreGO->AddComponent(new ScoreDisplay(gameLogicComp, scoreGO->GetComponent<dae::TextComponent>()));

	return scoreGO;
}

std::shared_ptr<dae::GameObject> MakeNumberSquare(int number, int boardPosIdx)
{
	const auto numberTextOffsetX = 8.f;
	const auto numberTextOffsetY = 10.f;
	const auto squareSize = 70.f;
	const auto spaceBetweenSquares = 9.6f;
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);

	const auto colIdx = boardPosIdx / 4;
	auto rowIdx = boardPosIdx % 4 - 1;
	if (rowIdx == -1)
		rowIdx = 3;

	const auto posX = g_BoardStartX + spaceBetweenSquares + float(rowIdx) * (squareSize + spaceBetweenSquares);
	const auto posY = g_BoardStartY + spaceBetweenSquares + float(colIdx) * (squareSize + spaceBetweenSquares);

	auto numberSquareGO = std::make_shared<dae::GameObject>();
	numberSquareGO->AddComponent(new NumberSquare(numberSquareGO, number, boardPosIdx));
	numberSquareGO->AddComponent(new dae::GraphicsComponent("Number Square.png", posX, posY, squareSize, squareSize));
	numberSquareGO->AddComponent(new dae::TextComponent(std::to_string(number), font, 119, 110, 101));
	numberSquareGO->GetComponent<dae::TextComponent>()->SetPosition(posX + squareSize / 2.f - numberTextOffsetX, posY + squareSize / 2.f - numberTextOffsetY);
	
	return numberSquareGO;
}

std::shared_ptr<dae::GameObject> MakeFPSCounter()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);
	auto fpsCounterGO = std::make_shared<dae::GameObject>();
	fpsCounterGO->AddComponent(new dae::FPSComponent(fpsCounterGO));
	fpsCounterGO->AddComponent(new dae::TextComponent("FAIL FPS", font, 255, 255, 0));
	fpsCounterGO->GetComponent<dae::TextComponent>()->SetPosition(10, 10);

	return fpsCounterGO;
}