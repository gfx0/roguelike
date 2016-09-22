#include "OperatingSystem.h"

/*****************************************************************************

	Purpose of this file is to provide access into the operating systems
	files, folders and special features, but not just limit itself to file io.

	Also important is to utilize anything needed from OS Entrypoint parameters.

 *****************************************************************************/

#include <SDL.h>
#include <stdio.h>



/*****************************************************************************
                                               ___                              
                                              / __)              _              
 ____  _____  ___  ___ _   _     ____ _____ _| |__ _____  ____ _| |_ ___   ____ 
|    \| ___ |/___)/___) | | |   / ___) ___ (_   __|____ |/ ___|_   _) _ \ / ___)
| | | | ____|___ |___ | |_| |  | |   | ____| | |  / ___ ( (___  | || |_| | |    
|_|_|_|_____|___/(___/ \__  |  |_|   |_____) |_|  \_____|\____)  \__)___/|_|    
                      (____/                                                    
 *****************************************************************************/

//TODO: Add ifdefs for other platforms, use #WIN32 and __linux__, mac users need to suck on their thumbs for a while.
#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h> //+X11/Xlib.h ?
#else
#error "Platform not yet supported!"
#endif

int g_initOS(int argCount, char * argValues[])
{
	if (argCount > 1) return printf("This program doesn't accept parameters yet.\n"), 1;

#ifdef _WIN32
#ifndef _DEBUG
	HWND wnd = GetConsoleWindow();
	ShowWindow(wnd, false);
#else
	printf("You are running in debug mode, enjoy this fugly console window! :D\n");
#endif
#endif

	//Initialization parameters fed into the program from the OS were acceptable, return 0.
	return 0;
}
/*****************************************************************************
                                               ___                              
                                              / __)              _              
 ____  _____  ___  ___ _   _     ____ _____ _| |__ _____  ____ _| |_ ___   ____ 
|    \| ___ |/___)/___) | | |   / ___) ___ (_   __|____ |/ ___|_   _) _ \ / ___)
| | | | ____|___ |___ | |_| |  | |   | ____| | |  / ___ ( (___  | || |_| | |    
|_|_|_|_____|___/(___/ \__  |  |_|   |_____) |_|  \_____|\____)  \__)___/|_|    
                      (____/                                                    
*****************************************************************************/

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


void OperatingSystem::Update( unsigned int deltaTime )
{

}

char * OperatingSystem::GetBasePath()
{
	return SDL_GetBasePath();
}