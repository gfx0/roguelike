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

	char *resourceBasePath = mpOperatingSystem->GetBasePath();
	char mainMenuBackground[1024];
	sprintf(mainMenuBackground, "%sgfx\\mainmenu.png", resourceBasePath);

	//TODO: Pool these texture pointers into the image loader / image manager instead of managing them like this.
	mpMainMenuBackgroundImage = mpImageLoader->LoadTexture(mainMenuBackground, mpRenderingSystem->GetRenderer());

	SDL_Color color = { 255, 255, 255, 255 };
	mpMainMenuOptionsTexts = mpFontLoader->RenderText("1. Start Game 2. Options 3. Quit", FontLoader::FONT_SAMPLE, color, 32, mpRenderingSystem->GetRenderer());
	if (mpMainMenuOptionsTexts == nullptr)
	{
		Game::GetGame()->Shutdown("Failed to create a font text texture for showing the testing text of this demo..\n");
		return;
	}

	mpGameTitle = mpFontLoader->RenderText("GAME NAME HERE", FontLoader::FONT_SAMPLE, color, 32, mpRenderingSystem->GetRenderer());
	if (mpGameTitle == nullptr)
	{
		Game::GetGame()->Shutdown("Failed to create a font text texture for showing the testing text of this demo..\n");
		return;
	}

}

void MainMenuState::OnExit()
{
	if (mpMainMenuBackgroundImage)
		SDL_DestroyTexture(mpMainMenuBackgroundImage);
	if (mpGameTitle)
		SDL_DestroyTexture(mpGameTitle);
	if (mpMainMenuOptionsTexts)
		SDL_DestroyTexture(mpMainMenuOptionsTexts);
}

void MainMenuState::OnUpdate( unsigned int deltaTime )
{
	//Render the scene
	mpRenderingSystem->Clear();
	mpRenderingSystem->RenderTexture(mpMainMenuBackgroundImage, 0, 0);
	mpRenderingSystem->RenderTexture(mpMainMenuOptionsTexts, 1200/2-192, 720/2);
	mpRenderingSystem->RenderTexture(mpGameTitle, 1280/2-128, 128);
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
			Game::GetGame()->GetStateMachine()->TransitionTo(GAMESTATE_PLAYGAME);
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