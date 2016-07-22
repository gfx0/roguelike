#include "InitGameState.h"
#include "settings.h"
#include "Game.h"

#include "RenderingSystem.h"
#include "ImageLoader.h"
#include "FontLoader.h"
#include "OperatingSystem.h"

#include <time.h> //srand

#include <stdio.h>

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <assert.h>

/*************************************************************
*
* This game state initializes a new game for the player.
*
* What that means, is that it loads the level and initializes
* any and all data necessary for the player and the enemies.
*
*************************************************************/
InitGameState::InitGameState()
	: mIsNewGameLoading(true)
	, mpRenderingSystem(NULL)
	, mpFontLoader(NULL)
	, mpImageLoader(NULL)
	, mTimeToWaitUntilTransitioningMS(2000)
	//TODO: Initialize the rest of the demo member variables, blah tedious...
{

}

InitGameState::~InitGameState()
{

}

void InitGameState::OnEnter()
{
	mIsNewGameLoading = true;

	/****************************************************************************************************************

	__          ______  _____  _  __  _____ _   _   _____  _____   ____   _____ _____  ______  _____ _____ 
	\ \        / / __ \|  __ \| |/ / |_   _| \ | | |  __ \|  __ \ / __ \ / ____|  __ \|  ____|/ ____/ ____|
	 \ \  /\  / / |  | | |__) | ' /    | | |  \| | | |__) | |__) | |  | | |  __| |__) | |__  | (___| (___  
	  \ \/  \/ /| |  | |  _  /|  <     | | | . ` | |  ___/|  _  /| |  | | | |_ |  _  /|  __|  \___ \\___ \ 
	   \  /\  / | |__| | | \ \| . \   _| |_| |\  | | |    | | \ \| |__| | |__| | | \ \| |____ ____) |___) |
	    \/  \/   \____/|_|  \_\_|\_\ |_____|_| \_| |_|    |_|  \_\\____/ \_____|_|  \_\______|_____/_____/ 

	 ****************************************************************************************************************/

	/****************************************************
	*
	* TODO:
	*
	*	Write cleanups for all textures separately.
	*	Perhaps a TextureManager class which stores
	*	all textures persistently in memory anyway?
	*
	*	Order of cleanup / destructing objects is
	*	important, don't just write one cleanup function
	*	for all types without knowing the order in which
	*	memory allocated SDL objects should be destroyed in.
	*
	* TODO:
	*
	*	Create a TextRenderingManager or something.
	*
	*	Purpose:
	*
	*		1. Static texts rendered into one texture
	*		2. Bitmap fonts for fast changing texts?
	*
	*		A combination of these two techniques should
	*		be used for efficient text rendering if
	*		realtime updating text fields are wanted in
	*		this game project. Perhaps damage / hit
	*		indicators / floating combat text?
	*
	*		See this for font atlas creation:
	*		http://www.angelcode.com/products/bmfont/
	*
	****************************************************/

	mpOperatingSystem = Game::GetGame()->GetEngineComponent<OperatingSystem>("OperatingSystem");
	mpRenderingSystem = Game::GetGame()->GetEngineComponent<RenderingSystem>("RenderingSystem");
	mpImageLoader = Game::GetGame()->GetEngineComponent<ImageLoader>("ImageLoader");
	mpFontLoader = Game::GetGame()->GetEngineComponent<FontLoader>("FontLoader");

	WindowCreationInfo info;
	info.x = SDL_WINDOWPOS_CENTERED;
	info.y = SDL_WINDOWPOS_CENTERED;
	info.w = 1280; // GetSystemMetrics(SM_CXSCREEN);
	info.h = 720; // GetSystemMetrics(SM_CYSCREEN);
	info.borderless = true;
	info.fullscreen = false;

	if ( mpRenderingSystem->CreateWindow(info) != 0 )
		return Game::GetGame()->Shutdown("Could not create a window, shutting down...\n");

	/**********************************
	*	Font loading
	**********************************/
	char *resourceBasePath = mpOperatingSystem->GetBasePath();
	char fullFilePath[2048] = "";
	sprintf(fullFilePath, "%sfonts\\sample.ttf", resourceBasePath);
	if (!mpFontLoader->LoadFont(fullFilePath, 32))
		Game::GetGame()->Shutdown("Could not load font :(\n");
	
	/**********************************
	*	Loading screen background image
	**********************************/
	char loadingScreenBackground[1024];
	sprintf(loadingScreenBackground, "%sgfx\\loadingscreenbackground.png", resourceBasePath);
	//TODO: Pool these texture pointers into the image loader / image manager instead of this kind of management.
	mpLoadingScreenBackground = mpImageLoader->LoadTexture(loadingScreenBackground, mpRenderingSystem->GetRenderer());

	SDL_Color color = { 0, 100, 0, 255 };
	char msg[512];
	sprintf(msg, "Systems initialized successfully. Engine version: 0.01");
	mpLoadingScreenTextMessage = mpFontLoader->RenderText(msg, FontLoader::FONT_SAMPLE, color, 32, mpRenderingSystem->GetRenderer());
	if (mpLoadingScreenTextMessage == nullptr)
	{
		Game::GetGame()->Shutdown("Failed to create a font text texture for showing the testing text of this demo..\n");
		return;
	}

	mpRenderingSystem->Clear();

	mIsNewGameLoading = false;

}

void InitGameState::OnExit()
{
	if ( mpLoadingScreenBackground )
		SDL_DestroyTexture(mpLoadingScreenBackground);
}

void InitGameState::OnUpdate( unsigned int deltaTime )
{
	/**********************************
	 * Still loading sprites etc?
	 **********************************/
	if ( mIsNewGameLoading )
		return;

	mpRenderingSystem->RenderTexture(mpLoadingScreenBackground, 0, 0);
	mpRenderingSystem->RenderTexture(mpLoadingScreenTextMessage, 128, 720 - 100);
	mpRenderingSystem->Render();


	/**********************************
	 * ! ARTIFICIAL TEMP DELAY !
	 **********************************/
	mTimeToWaitUntilTransitioningMS -= deltaTime;
	if ( mTimeToWaitUntilTransitioningMS <= 0 )
	{
		Game::GetGame()->GetStateMachine()->TransitionTo(GAMESTATE_MAINMENU);
	}

}

void InitGameState::OnInput(SDL_Event & pEvent)
{
	if (pEvent.type == SDL_QUIT)
		return Game::GetGame()->Shutdown("Quit pressed during loading screen??");

	if (pEvent.type == SDL_KEYDOWN)
	{
		switch (pEvent.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game::GetGame()->Shutdown("Quit pressed during loading screen??");
			break;
		default:
			break;
		}
	}
}