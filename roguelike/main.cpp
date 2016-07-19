#include "Game.h"
#include <windows.h>

int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{

	Game * pGame = new Game();

	pGame->Start();

	while (pGame->IsOn())
		pGame->Update();

	delete pGame;

	return 0;
}