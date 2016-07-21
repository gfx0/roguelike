#include "MainMenuState.h"
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
MainMenuState::MainMenuState()
	: mpRenderingSystem(NULL)
	, mpFontLoader(NULL)
	, mpImageLoader(NULL)
	, mpMainMenuBackgroundImage(NULL)
	, mpGameTitle(NULL)
	, mpMainMenuOptionsTexts(NULL)
{

}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::OnEnter()
{
	mpOperatingSystem = Game::GetGame()->GetEngineComponent<OperatingSystem>("OperatingSystem");
	mpRenderingSystem = Game::GetGame()->GetEngineComponent<RenderingSystem>("RenderingSystem");
	mpImageLoader = Game::GetGame()->GetEngineComponent<ImageLoader>("ImageLoader");
	mpFontLoader = Game::GetGame()->GetEngineComponent<FontLoader>("FontLoader");

	/**************************************************************************
	 *
	 *
	 *
	 *		TODO:		mpRenderer of RenderingSystem is invalid after first
	 *					creation... Solve with reference pointer or some such.
	 *
	 *
	 *
	 **************************************************************************/

	WindowCreationInfo info;
	info.x = SDL_WINDOWPOS_CENTERED;
	info.y = SDL_WINDOWPOS_CENTERED;
	info.w = 1280; // GetSystemMetrics(SM_CXSCREEN);
	info.h = 720; // GetSystemMetrics(SM_CYSCREEN);
	info.borderless = true;
	info.fullscreen = false;

	mMainMenuTextsX = 1280/2 - 200;
	mMainMenuTextsY = 720/2;

	if ( mpRenderingSystem->CreateWindow(info) != 0 )
		return Game::GetGame()->Shutdown("Could not create a window, shutting down...\n");

	char *resourceBasePath = mpOperatingSystem->GetBasePath();
	char mainMenuBackground[1024];
	sprintf(mainMenuBackground, "%sgfx\\mainmenu.png", resourceBasePath);

	//TODO: Pool these texture pointers into the image loader / image manager instead of keeping them on stack like this.
	mpMainMenuBackgroundImage = mpImageLoader->LoadTexture(mainMenuBackground, mpRenderingSystem->GetRenderer());
	
	/**********************************
	 *	Font loading
	 **********************************/
	char fullFilePath[2048] = "";
	sprintf(fullFilePath, "%sfonts\\sample.ttf", resourceBasePath);
	if ( !mpFontLoader->LoadFont(fullFilePath, 32) )
		Game::GetGame()->Shutdown("Could not load font :(\n");

	SDL_Color color = { 255, 255, 255, 255 };
	mpMainMenuOptionsTexts = mpFontLoader->RenderText("1. Start Game 2. Options 3. Quit", FontLoader::FONT_SAMPLE, color, 32, mpRenderingSystem->GetRenderer());
	if (mpMainMenuOptionsTexts == nullptr)
	{
		Game::GetGame()->Shutdown("Failed to create a font text texture for showing the testing text of this demo..\n");
		return;
	}

}

void MainMenuState::OnExit()
{
	//TODO: At the moment these are deleted by the game...
	/*
	delete mpOperatingSystem;
	delete mpImageLoader;
	delete mpFontLoader;
	delete mpRenderingSystem;
	*/
}

void MainMenuState::OnUpdate()
{

	//Render the scene
	mpRenderingSystem->Clear();
	mpRenderingSystem->RenderTexture(mpMainMenuBackgroundImage, 0, 0);
	mpRenderingSystem->RenderTexture(mpMainMenuOptionsTexts, mMainMenuTextsX, mMainMenuTextsY);
	mpRenderingSystem->Render();
}

void MainMenuState::OnInput(SDL_Event & pEvent)
{

	if (pEvent.type == SDL_QUIT)
	{
		Game::GetGame()->Shutdown("Quit pressed during loading screen??");
		return;
	}
	if (pEvent.type == SDL_KEYDOWN)
	{
		switch (pEvent.key.keysym.sym)
		{
		case SDLK_1:
			Game::GetGame()->GetStateMachine()->TransitionTo("PlayGameState");
			break;
		case SDLK_2:
			break;
		case SDLK_3:
			Game::GetGame()->Shutdown("User chose to quit from main menu.");
			break;
		case SDLK_ESCAPE:
			Game::GetGame()->Shutdown("Quit pressed during loading screen??");
			break;
		default:
			break;
		}
	}
}