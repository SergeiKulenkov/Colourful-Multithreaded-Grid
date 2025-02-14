#include "App/App.h"
#include "Game/Game.h"

int main()
{
	Game::Init("Colourful Multithreaded Grid");
	Game::Get().Run();
	Game::Shutdown();

	return 0;
}