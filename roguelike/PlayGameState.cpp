#include "PlayGameState.h"
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
PlayGameState::PlayGameState()
	: mIsNewGameLoading(true)
	, mpRenderingSystem(NULL)
	, mpFontLoader(NULL)
	, mpImageLoader(NULL)
	, mPlayerTempX(0)
	, mPlayerTempY(0)
	, mCachedWindowWidth(0)
	, mCachedWindowHeight(0)
	, mCachedWindowHeightHalf(0)
	, mCachedWindowWidthHalf(0)
{

}

PlayGameState::~PlayGameState()
{

}

void PlayGameState::OnEnter()
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

	/**************************
	 * Texture loading
	 **************************/
	char *resourceBasePath = mpOperatingSystem->GetBasePath();
	char grassTileImagePath[1024];
	sprintf(grassTileImagePath, "%sgfx\\grasstile.png", resourceBasePath);
	char wallTileImagePath[1024];
	sprintf(wallTileImagePath, "%sgfx\\walltile.png", resourceBasePath);
	char foregroundImagePath[1024];
	sprintf(foregroundImagePath, "%sgfx\\knight.png", resourceBasePath);
	char spriteSheetTestPath[1024];
	sprintf(spriteSheetTestPath, "%sgfx\\testspritesheet.png", resourceBasePath);

	mpWallTileTexture	= mpImageLoader->LoadTexture(wallTileImagePath,	mpRenderingSystem->GetRenderer());
	mpPlayerSprite		= mpImageLoader->LoadTexture(foregroundImagePath, mpRenderingSystem->GetRenderer());
	mpTestSpriteSheet	= mpImageLoader->LoadTexture(spriteSheetTestPath, mpRenderingSystem->GetRenderer());

	if (mpPlayerSprite == NULL || mpWallTileTexture == NULL || mpTestSpriteSheet == NULL) {
		SDL_DestroyTexture(mpTestSpriteSheet);
		SDL_DestroyTexture(mpWallTileTexture);
		SDL_DestroyTexture(mpPlayerSprite);
		Game::GetGame()->Shutdown("Could not load one or more of the necessary textures :( shutting down...\n");
		return;
	}

	mpRenderingSystem->Clear();

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

void PlayGameState::OnExit()
{
	if ( mpWallTileTexture )
		SDL_DestroyTexture(mpWallTileTexture);
	if ( mpTestSpriteSheet )
		SDL_DestroyTexture(mpTestSpriteSheet);
	if ( mpPlayerSprite )
		SDL_DestroyTexture(mpPlayerSprite);
}

void PlayGameState::OnUpdate( unsigned int deltaTime )
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
	mpRenderingSystem->RenderTexture(mpWallTileTexture, mCachedWindowWidthHalf, mCachedWindowHeightHalf);
	mpRenderingSystem->RenderTexture(mpPlayerSprite, mPlayerTempX, mPlayerTempY);
	mpRenderingSystem->RenderTexture(mpStatusBarImageFontTexture, 0, 0);//left top
	mpRenderingSystem->Render();
}

void PlayGameState::OnInput(SDL_Event & pEvent)
{
	//if ( !pEvent )
		//return;

	//if ( pEvent == NULL )
	//	return;

	if (pEvent.type == SDL_QUIT)
	{
		Game::GetGame()->Shutdown("Quit from playgamestate.");
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
			//TODO: Pause menu state?
			Game::GetGame()->GetStateMachine()->TransitionTo(GAMESTATE_MAINMENU);
			break;
		default:
			break;
		}
	}
	if (pEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		//Yeah don't shut down, but keep this here as an example of functionality.
		//Game::GetGame()->Shutdown("Quit from playgamestate.");
		//return;
	}
}