#include "OperatingSystem.h"

/*****************************************************************************

	Purpose of this file is to provide access into the operating systems
	files, folders and special features, but not just limit itself to file io.

 *****************************************************************************/

#include <SDL.h>
#include <stdio.h>

OperatingSystem::OperatingSystem()
{

}

OperatingSystem::~OperatingSystem()
{

}

bool OperatingSystem::Start()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}


void OperatingSystem::Stop()
{

}


void OperatingSystem::Update()
{

}

char * OperatingSystem::GetBasePath()
{
	return SDL_GetBasePath();
}