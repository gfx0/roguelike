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

#include "sdlhelpers.h"

#pragma optimize("", off)

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



	// TODO:		First show a 'loading ...' image
	//				Then load all the rest of the textures
	//				Load the font
	//				Initialize the game logic
	//				Transition to the play game state and actually use the initialized assets.

	// TODO:		Get rid of sdlhelpers.h
	//				Only cleanups remain,
	//				they should be done in each manager instead of these stack functions.

	// TODO:		Move t

	mpOperatingSystem = Game::GetGame()->GetEngineComponent<OperatingSystem>("OperatingSystem");
	mpRenderingSystem = Game::GetGame()->GetEngineComponent<RenderingSystem>("RenderingSystem");
	mpImageLoader = Game::GetGame()->GetEngineComponent<ImageLoader>("ImageLoader");
	mpFontLoader = Game::GetGame()->GetEngineComponent<FontLoader>("FontLoader");

	if ( mpRenderingSystem->CreateWindow(100, 100, 1280, 720) != 0 )
		return Game::GetGame()->Shutdown("Could not create a window, shutting down...\n");


	/**********************************
	 *	Font loading
	 **********************************/
	char *resourceBasePath = mpOperatingSystem->GetBasePath();
	char fullFilePath[2048] = "";
	sprintf(fullFilePath, "%sfonts\\sample.ttf", resourceBasePath);
	if ( !mpFontLoader->LoadFont(fullFilePath, 32) )
		Game::GetGame()->Shutdown("Could not load font :(\n");

	SDL_Color color = { 255, 255, 255, 255 };
	mpStatusBarImageFontTexture = mpFontLoader->RenderText("Level: 1 Int: 1 Str: 1 Dex: 1", FontLoader::FONT_SAMPLE, color, 32, mpRenderingSystem->GetRenderer());
	if (mpStatusBarImageFontTexture == nullptr)
	{
		Game::GetGame()->Shutdown("Failed to create a font text texture for showing the testing text of this demo..\n");
		return;
	}

	/**************************
	 * Texture loading
	 **************************/
	char grassTileImagePath[1024];
	sprintf(grassTileImagePath, "%sgfx\\grasstile.png", resourceBasePath);
	char wallTileImagePath[1024];
	sprintf(wallTileImagePath, "%sgfx\\walltile.png", resourceBasePath);
	char foregroundImagePath[1024];
	sprintf(foregroundImagePath, "%sgfx\\knight.png", resourceBasePath);
	char spriteSheetTestPath[1024];
	sprintf(spriteSheetTestPath, "%sgfx\\testspritesheet.png", resourceBasePath);

	//TODO: Pool these texture pointers into the image loader / image manager instead of keeping them on stack like this.
	mpBackground		= mpImageLoader->LoadTexture(grassTileImagePath, mpRenderingSystem->GetRenderer());
	mpWallTileTexture	= mpImageLoader->LoadTexture(wallTileImagePath,	mpRenderingSystem->GetRenderer());
	mpPlayerSprite		= mpImageLoader->LoadTexture(foregroundImagePath, mpRenderingSystem->GetRenderer());
	mpTestSpriteSheet	= mpImageLoader->LoadTexture(spriteSheetTestPath, mpRenderingSystem->GetRenderer());

	if (mpBackground == NULL || mpPlayerSprite == NULL || mpWallTileTexture == NULL || mpTestSpriteSheet == NULL) {
		cleanup(mpTestSpriteSheet);
		cleanup(mpWallTileTexture);
		cleanup(mpBackground);
		cleanup(mpPlayerSprite);
		Game::GetGame()->Shutdown("Could not load one or more of the necessary textures :( shutting down...\n");
		return;
	}

	mpRenderingSystem->Clear();

	mCachedWindowWidth	= mpRenderingSystem->GetWindowWidth();
	mCachedWindowHeight = mpRenderingSystem->GetWindowHeight();

	mCachedWindowWidthHalf = mCachedWindowWidth / 2;
	mCachedWindowHeightHalf = mCachedWindowHeight / 2;
	/**********************************
	* Player Position (center screen)
	**********************************/
	mPlayerTempX = mCachedWindowWidth / 2;
	mPlayerTempY = mCachedWindowHeight / 2;

	mIsNewGameLoading = false;

	/****************************************************************************************************************

	__          ______  _____  _  __  _____ _   _   _____  _____   ____   _____ _____  ______  _____ _____ 
	\ \        / / __ \|  __ \| |/ / |_   _| \ | | |  __ \|  __ \ / __ \ / ____|  __ \|  ____|/ ____/ ____|
	 \ \  /\  / / |  | | |__) | ' /    | | |  \| | | |__) | |__) | |  | | |  __| |__) | |__  | (___| (___  
	  \ \/  \/ /| |  | |  _  /|  <     | | | . ` | |  ___/|  _  /| |  | | | |_ |  _  /|  __|  \___ \\___ \ 
	   \  /\  / | |__| | | \ \| . \   _| |_| |\  | | |    | | \ \| |__| | |__| | | \ \| |____ ____) |___) |
	    \/  \/   \____/|_|  \_\_|\_\ |_____|_| \_| |_|    |_|  \_\\____/ \_____|_|  \_\______|_____/_____/ 

	 ****************************************************************************************************************/
}

void InitGameState::OnExit()
{
	delete mpRenderingSystem;
}

void InitGameState::OnUpdate()
{
	/**********************************
	 * Still loading sprites etc?
	 **********************************/
	if ( mIsNewGameLoading )
		return;

	//Render the scene
	mpRenderingSystem->Clear();
	//Draw the tiles by calculating their positions
	/**************************************************************************
	 *
	 *	TODO:	Make a level loader?
	 *			First it can just load 
	 *			ascii file like this:
	 *
	 *
	 *			Example of basic level?
	 *
	 *			#####################
	 *			#					#
	 *			#				#	#
	 *			#				#	#
	 *			########	####	#
	 *			#	   #	#		#
	 *			#	#  #	#		#
	 *			#	#		#		#
	 *			#####################
	 *
	 *	TODO:	Make a level editor?
	 *
	 *			1. Mouse click switches tile
	 *
	 *			1. Hover highlights editable tile
	 *
	 *			Escape saves the file?
	 *
	 *			Very simple!
	 *			
	 **************************************************************************/
	mpRenderingSystem->RenderTexture(mpPlayerSprite, mPlayerTempX, mPlayerTempY);
	mpRenderingSystem->RenderTexture(mpStatusBarImageFontTexture, 0, 0);//left top
	mpRenderingSystem->RenderTexture(mpWallTileTexture, mCachedWindowWidthHalf, mCachedWindowHeightHalf);
	mpRenderingSystem->Render();
}

void InitGameState::OnInput(SDL_Event & pEvent)
{
	//if ( !pEvent )
		//return;

	//if ( pEvent == NULL )
	//	return;

	if (pEvent.type == SDL_QUIT)
	{
		HackyQuit();
		return;
	}
	if (pEvent.type == SDL_KEYDOWN)
	{
		switch (pEvent.key.keysym.sym)
		{
		case SDLK_LEFT:
			mPlayerTempX -= 40;
			break;
		case SDLK_RIGHT:
			mPlayerTempX += 40;
			break;
		case SDLK_UP:
			mPlayerTempY -= 40;
			break;
		case SDLK_DOWN:
			mPlayerTempY += 40;
			break;
		case SDLK_1:
			break;
		case SDLK_2:
			break;
		case SDLK_3:
			break;
		case SDLK_4:
			break;
		case SDLK_ESCAPE:
			HackyQuit();
			break;
		default:
			break;
		}
	}
	if (pEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		HackyQuit();
		return;
	}
}

//NOTE: Game should shutdown nicely via OnExit, but for this demo purpose this should be fine.
void InitGameState::HackyQuit()
{
	if ( mpWallTileTexture )
		cleanup(mpWallTileTexture);
	if ( mpTestSpriteSheet )
		cleanup(mpTestSpriteSheet);
	if ( mpPlayerSprite )
		cleanup(mpPlayerSprite);
	if ( mpBackground )
		cleanup(mpBackground);

	Game::GetGame()->Shutdown("Hacky quit shutdown!");
}


void InitGameState::LoadNewLevelAndStartGame()
{
	/*
	if (!mpPlayerManager->GetPlayer(0)->GetIsSpriteSet())
		mpPlayerManager->GetPlayer(0)->SetSprite(mpSpriteManager->GetSprite("data/player.bmp"));

	//TODO: Load enemies and set their positions, they are the 'level'.
	//TODO: Load enemy positions from a file, create a level manager for this.
	int numberOfAlienRows = (rand() % SETTINGS_MAX_ROWS_OF_ALIENS) + SETTINGS_MIN_ROWS_OF_ALIENS;
	for (int columnIndex = 0; columnIndex < (SETTINGS_SCREEN_WIDTH / SETTINGS_SPRITE_WIDTH) - 2; columnIndex++)
		for (int rowIndex = 0; rowIndex < numberOfAlienRows; rowIndex++)
			if (rand() % 2 == 1)
				mpEnemyManager->CreateEnemyAtPosition(SETTINGS_SPRITE_WIDTH + columnIndex*SETTINGS_SPRITE_WIDTH,
					rowIndex*SETTINGS_SPRITE_HEIGHT,
					rand() % 2 == 1 ? EnemyManager::ENEMY_GREEN_ALIEN : EnemyManager::ENEMY_RED_ALIEN);


	Game::GetGame()->GetStateMachine()->TransitionTo("StartTheGame");
	*/
}
