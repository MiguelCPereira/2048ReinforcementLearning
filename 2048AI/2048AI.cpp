// Project includes
#define SDL_MAIN_HANDLED
#include <ctime>
#include <Minigin.h>
#include <string>
#include "Factory.h"
#include "GameObject.h"
#include "NeuralNetwork.h"
#include "Scene.h"

void SetUpGame();

int main(int, char* [])
{
	srand((unsigned)time(nullptr));
	
	dae::Minigin engine;
	engine.Initialize();
	SetUpGame();
	
	engine.Run();

	return 0;
}

void SetUpGame()
{
	// Create the scene
	std::string sceneName = "2048Scene";
	auto& scene = dae::SceneManager::GetInstance().CreateScene(sceneName);

	// Draw the background
	const auto backgroundGO = MakeBackground();
	scene.Add(backgroundGO);

	// Make the game logic
	const auto gameLogicGO = MakeGameLogic();
	scene.Add(gameLogicGO);

	// Make the score display
	scene.Add(MakeScoreDisplay(gameLogicGO));

	// Set up AI
	scene.Add(MakePlayerAI(gameLogicGO, 0.0001f));

	// Set up the temporary player input --- to set this one up, comment out the previous line
	//SetUpPlayerInput(gameLogicGO);
	
}
