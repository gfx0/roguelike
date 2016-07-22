#include "Game.h"

//NOTE: Add ifdefs for other platforms here and gg, things "should"(tm) work out of the box for mac and linux. (ofc main can't be winmain lol)
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