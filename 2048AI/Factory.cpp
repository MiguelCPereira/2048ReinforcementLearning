#include "Factory.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "ResourceManager.h"
#include "FPSComponent.h"
#include "GameCommands.h"
#include "GameLogic.h"
#include "InputManager.h"
#include "NumberSquare.h"
#include "ScoreDisplay.h"
#include "TextComponent.h"
//#include "ScoreDisplay.h"

const auto g_BoardStartX = 155.f;
const auto g_BoardStartY = 80.f;
const auto g_SquareSize = 70.f;
const auto g_SpaceBetweenSquares = 9.6f;

std::shared_ptr<dae::GameObject> MakeGameLogic()
{
	auto gameLogicGO = std::make_shared<dae::GameObject>();
	gameLogicGO->AddComponent(new GameLogic(gameLogicGO, g_SquareSize + g_SpaceBetweenSquares));

	return gameLogicGO;
}

std::shared_ptr<dae::GameObject> MakeBackground()
{
	const auto backgWidthHeight = 326.f;
	
	auto backgroundGO = std::make_shared<dae::GameObject>();
	backgroundGO->AddComponent(new dae::GraphicsComponent("Board.png", g_BoardStartX, g_BoardStartY, backgWidthHeight, backgWidthHeight));
	
	return backgroundGO;
}

std::shared_ptr<dae::GameObject> MakeScoreDisplay(const std::shared_ptr<dae::GameObject>& gameLogic)
{
	auto* gameLogicComp = gameLogic->GetComponent<GameLogic>();
	const auto posX = 20.f;
	const auto posY = 20.f;

	auto scoreGO = std::make_shared<dae::GameObject>();

	const auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 23);
	scoreGO->AddComponent(new dae::TextComponent("SCORE: FAIL", font, 238, 228, 218));
	scoreGO->GetComponent<dae::TextComponent>()->SetPosition(posX, posY);
	scoreGO->AddComponent(new ScoreDisplay(gameLogicComp, scoreGO->GetComponent<dae::TextComponent>()));

	return scoreGO;
}

std::shared_ptr<dae::GameObject> MakeNumberSquare(int number, int rowIdx, int colIdx)
{
	const auto numberTextOffsetX = 8.f;
	const auto numberTextOffsetY = 10.f;
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);

	const auto posX = g_BoardStartX + g_SpaceBetweenSquares + float(colIdx) * (g_SquareSize + g_SpaceBetweenSquares);
	const auto posY = g_BoardStartY + g_SpaceBetweenSquares + float(rowIdx) * (g_SquareSize + g_SpaceBetweenSquares);

	auto numberSquareGO = std::make_shared<dae::GameObject>();
	numberSquareGO->AddComponent(new NumberSquare(numberSquareGO, number, rowIdx, colIdx, g_SquareSize + g_SpaceBetweenSquares));
	numberSquareGO->AddComponent(new dae::GraphicsComponent("Number Square.png", posX, posY, g_SquareSize, g_SquareSize));
	numberSquareGO->AddComponent(new dae::TextComponent(std::to_string(number), font, 119, 110, 101));
	numberSquareGO->GetComponent<dae::TextComponent>()->SetPosition(posX + g_SquareSize / 2.f - numberTextOffsetX, posY + g_SquareSize / 2.f - numberTextOffsetY);
	
	return numberSquareGO;
}

std::shared_ptr<dae::GameObject> MakeFPSCounter()
{
	const auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);
	auto fpsCounterGO = std::make_shared<dae::GameObject>();
	fpsCounterGO->AddComponent(new dae::FPSComponent(fpsCounterGO));
	fpsCounterGO->AddComponent(new dae::TextComponent("FAIL FPS", font, 255, 255, 0));
	fpsCounterGO->GetComponent<dae::TextComponent>()->SetPosition(10, 10);

	return fpsCounterGO;
}

void SetUpPlayerInput(std::shared_ptr<dae::GameObject> gameLogic)
{
	auto moveUpKeyboard = std::make_unique<SwipeUpCommand>();
	moveUpKeyboard->SetActor(gameLogic);
	moveUpKeyboard->SetButtonPressType(dae::ButtonPress::PressedDown);
	dae::InputManager::GetInstance().AddCommand(SDLK_UP, std::move(moveUpKeyboard));

	auto moveDownKeyboard = std::make_unique<SwipeDownCommand>();
	moveDownKeyboard->SetActor(gameLogic);
	moveDownKeyboard->SetButtonPressType(dae::ButtonPress::PressedDown);
	dae::InputManager::GetInstance().AddCommand(SDLK_DOWN, std::move(moveDownKeyboard));

	auto moveLeftKeyboard = std::make_unique<SwipeLeftCommand>();
	moveLeftKeyboard->SetActor(gameLogic);
	moveLeftKeyboard->SetButtonPressType(dae::ButtonPress::PressedDown);
	dae::InputManager::GetInstance().AddCommand(SDLK_LEFT, std::move(moveLeftKeyboard));

	auto moveRightKeyboard = std::make_unique<SwipeRightCommand>();
	moveRightKeyboard->SetActor(gameLogic);
	moveRightKeyboard->SetButtonPressType(dae::ButtonPress::PressedDown);
	dae::InputManager::GetInstance().AddCommand(SDLK_RIGHT, std::move(moveRightKeyboard));

	auto restartKeyboard = std::make_unique<RestartCommand>();
	restartKeyboard->SetActor(gameLogic);
	restartKeyboard->SetButtonPressType(dae::ButtonPress::PressedDown);
	dae::InputManager::GetInstance().AddCommand(SDLK_r, std::move(restartKeyboard));
}