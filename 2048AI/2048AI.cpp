// Project includes
#define SDL_MAIN_HANDLED
#include <Minigin.h>

int main(int, char* [])
{
	dae::Minigin engine;
	engine.Initialize();
	engine.LoadDemo();
	engine.Run();

	return 0;
}
