#include "Game.h"

int main(int argc, char* args[])
{
	if (Game::Instance()->Init("SDL Shooting", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED, 320, 320, 0) == false) return 1;
		                       //SDL_WINDOWPOS_CENTERED, 800, 600, 0) == false) return 1;
	while (Game::Instance()->Running())
	{
		Game::Instance()->HandleEvents();
		if (Game::Instance()->Tick())
		{  
			Game::Instance()->Update();
			Game::Instance()->Render();
		}
	}
	Game::Instance()->Clean();
	return 0;
}