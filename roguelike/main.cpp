#include "Game.h"
#include "OperatingSystem.h"

int main(int argCount, char *argValues[])
{

	if (int result = g_initOS(argCount, argValues) != 0)
		return result;

	Game * pGame = new Game();
	pGame->Start();

	while (pGame->IsOn())
		pGame->Update();

	delete pGame;

	return 0;
}
