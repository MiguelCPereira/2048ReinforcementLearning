// Project includes
#define SDL_MAIN_HANDLED
#include <Minigin.h>
#include <string>
#include "Factory.h"
#include "GameObject.h"
#include "Scene.h"
//#include "GameObject.h"
//#include "Command.h"
//#include "InputManager.h"
//#include "GameCommands.h"

void SetUpGame();

int main(int, char* [])
{
	dae::Minigin engine;
	engine.Initialize();
	//engine.LoadDemo();
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

	// Make the score display
	scene.Add(MakeScoreDisplay());

	// Make some test numberSquares
	scene.Add(MakeNumberSquare(2, 1));
	scene.Add(MakeNumberSquare(4, 7));
	scene.Add(MakeNumberSquare(8, 2));
	scene.Add(MakeNumberSquare(2, 12));
}
