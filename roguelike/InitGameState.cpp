#include "InitGameState.h"
#include "settings.h"
#include "Game.h"

#include "RenderingSystem.h"
#include "ImageLoader.h"
#include "FontLoader.h"
#include "OperatingSystem.h"

#include <windows.h>
#undef CreateWindow // Fuck off windows

#include <time.h> //srand

#include <stdio.h>

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <assert.h>

#include "sdlhelpers.h"

//NOTE: These square tiles won't scale with 16:9 etc.
const int TILE_SIZE = 40;

/*************************************************************
*
* This game state initializes a new game for the player.
*
* What that means, is that it loads the level and initializes
* any and all data necessary for the player and the enemies.
*
*************************************************************/
InitGameState::InitGameState()
	: mIsNewGameLoading(false)
	, mIsNewGameSessionInitialized(false)
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
	//
	//	NOTE: Game engine currently gets stuck here since input is not yet created into its own system
	//
	//	TODO:	Create InputManager ?
	//
	//			States should have some kind of OnInput ( InputEvent ) so SDL Input Events can be handled non blockingly
	//			without the stupid while loop this HackySDLGameStart does!
	//
	HackySDLGameStart();

	/*
	mIsNewGameSessionInitialized = false;
	mIsNewGameLoading = false;
	mpRenderingSystem = Game::GetGame()->GetGameComponentManager()->GetComponent<RenderingSystem>("OldRenderingSystem");
	mpPlayerManager = Game::GetGame()->GetGameComponentManager()->GetComponent<PlayerManager>("PlayerManager");
	mpSpriteManager = Game::GetGame()->GetGameComponentManager()->GetComponent<SpriteManager>("SpriteManager");
	mpEnemyManager = Game::GetGame()->GetGameComponentManager()->GetComponent<EnemyManager>("EnemyManager");
	mpProjectileManager = Game::GetGame()->GetGameComponentManager()->GetComponent<ProjectileManager>("ProjectileManager");

	mpPlayerManager->GetPlayer(0)->SetX(SETTINGS_PLAYER_START_X);
	mpPlayerManager->GetPlayer(0)->SetY(SETTINGS_PLAYER_START_Y);

	//NOTE: OLD! (2013)
	if (!mpRenderingSystem->IsInitialized())
	{
		mpRenderingSystem->InitWindow(SETTINGS_SCREEN_WIDTH, SETTINGS_SCREEN_HEIGHT);
		mpSpriteManager->LoadSprite("data/player.bmp");
		mpSpriteManager->LoadSprite("data/enemy1.bmp");
		mpSpriteManager->LoadSprite("data/enemy2.bmp");
		mpSpriteManager->LoadSprite("data/rocket.bmp");
		mpSpriteManager->LoadSprite("data/bomb.bmp");
	}

	//If player either lost the game or the game is being initialized, give him some health.
	if (mpPlayerManager->GetPlayer(0)->GetHealth() <= 0)
		mpPlayerManager->GetPlayer(0)->SetHealth(SETTINGS_PLAYER_MAX_HEALTH);

	//Used when the player gets hit, temporary invulnerability :)
	mpPlayerManager->GetPlayer(0)->SetIsInvulnerable(false);

	//If any enemies from the previous session are alive, kill them now : o )
	mpEnemyManager->KillAllEnemies();
	mpProjectileManager->DestroyAllProjectiles();

	srand(time(NULL));

	mIsNewGameSessionInitialized = true;
	*/
}

void InitGameState::OnExit()
{
	//delete mpRenderingSystem; ?
}

void InitGameState::OnUpdate()
{
	/*
	if (mIsNewGameSessionInitialized && !mIsNewGameLoading)
	{
		mIsNewGameLoading = true;
		LoadNewLevelAndStartGame();
	}

	mpRenderingSystem->DrawSomeText(240, 200, "Loading new game..."); ?
	*/
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

void InitGameState::HackySDLGameStart()
{

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

	WindowCreationInfo info;
	info.x = SDL_WINDOWPOS_CENTERED;
	info.y = SDL_WINDOWPOS_CENTERED;
	info.w = 800; // GetSystemMetrics(SM_CXSCREEN);
	info.h = 600; // GetSystemMetrics(SM_CYSCREEN);
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
	if ( !mpFontLoader->LoadFont(fullFilePath, 32) )
		Game::GetGame()->Shutdown("Could not load font :(\n");

	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture *statusBarImageFontTexture = mpFontLoader->RenderText("Level: 1 Int: 1 Str: 1 Dex: 1", FontLoader::FONT_SAMPLE, color, 32, mpRenderingSystem->GetRenderer());
	if (statusBarImageFontTexture == nullptr)
	{
		Game::GetGame()->Shutdown("Failed to create a font text texture for showing the testing text of this demo..\n");
		return;
	}

	//Get the texture w/h so we can center it in the screen
	int iTextW, iTextH;
	SDL_QueryTexture(statusBarImageFontTexture, NULL, NULL, &iTextW, &iTextH);
	int xText = 0;//SCREEN_WIDTH / 2 - iTextW / 2;
	int yText = 0;// SCREEN_HEIGHT / 2 - iTextH / 2;



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
	SDL_Texture *background =		mpImageLoader->LoadTexture(grassTileImagePath,	mpRenderingSystem->GetRenderer());
	SDL_Texture *wallTileTexture =	mpImageLoader->LoadTexture(wallTileImagePath,	mpRenderingSystem->GetRenderer());
	SDL_Texture *playerSprite =		mpImageLoader->LoadTexture(foregroundImagePath, mpRenderingSystem->GetRenderer());
	SDL_Texture *testSpriteSheet =	mpImageLoader->LoadTexture(spriteSheetTestPath, mpRenderingSystem->GetRenderer());

	if (background == NULL || playerSprite == NULL || wallTileTexture == NULL || testSpriteSheet == NULL) {
		cleanup(testSpriteSheet);
		cleanup(wallTileTexture);
		cleanup(background);
		cleanup(playerSprite);
		Game::GetGame()->Shutdown("Could not load one or more of the necessary textures :( shutting down...\n");
		return;
	}

	mpRenderingSystem->Clear();

	int windowWidth = mpRenderingSystem->GetWindowWidth();
	int windowHeight = mpRenderingSystem->GetWindowHeight();

	/**************************
	* Player Position & Size
	**************************/
	int xTiles = windowWidth / TILE_SIZE;
	int yTiles = windowHeight / TILE_SIZE;

	int iW, iH;
	SDL_QueryTexture(playerSprite, NULL, NULL, &iW, &iH);
	int x = windowWidth / 2 - iW / 2;
	int y = windowHeight / 2 - iH / 2;

	/**************************
	* Sprite Sheet Pos ^ Size
	**************************/
	//iW and iH are the clip width and height
	//We'll be drawing only clips so get a center position for the w/h of a clip
	int iiW = 100, iiH = 100;
	int xx = windowWidth / 2 - iiW / 2;
	int yy = windowHeight / 2 - iiH / 2;

	//Setup the clips for our image
	SDL_Rect clips[4];
	for (int i = 0; i < 4; ++i) {
		clips[i].x = i / 2 * iiW;
		clips[i].y = i % 2 * iiH;
		clips[i].w = iiW;
		clips[i].h = iiH;
	}
	//Specify a default clip to start with
	int useClip = 0;


	//TODO: This needs to be non blocking.
	//
	//		No while loops inside of the OnUpdate()
	//
	//		OnUpdate should simply ask the renderer to render.... objects?
	//
	//				 (SDL_PollEvent)
	//		OnInput(    InputEvent   ) should handle input coming to the state.
	//
	//								Perhaps oninput could modify the states(positions) of these temp objects?
	//
	//								These state objects would be the player etc. in member variables of the state?
	//								
	//								Perhaps better in their own CharacterManager? hmm test it out.
	//
	//


	/**************************
	* SDL Events Loop
	**************************/
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
					x -= 40;
					break;
				case SDLK_RIGHT:
					x += 40;
					break;
				case SDLK_UP:
					y -= 40;
					break;
				case SDLK_DOWN:
					y += 40;
					break;
				case SDLK_1:
					useClip = 0;
					break;
				case SDLK_2:
					useClip = 1;
					break;
				case SDLK_3:
					useClip = 2;
					break;
				case SDLK_4:
					useClip = 3;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				quit = true;
			}
		}
		//Render the scene
		mpRenderingSystem->Clear();
		//Draw the tiles by calculating their positions
		/**********************************
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
		 **********************************/
		for (int i = 0; i < xTiles * yTiles; ++i) {
			int x = i % xTiles;
			int y = i / xTiles;
			//mpRenderingSystem->RenderTexture(background, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
		//renderTexture(testSpriteSheet, renderer, xx, yy, &clips[useClip]);
		mpRenderingSystem->RenderTexture(wallTileTexture, windowWidth / 2, windowHeight / 2);
		mpRenderingSystem->RenderTexture(playerSprite, x, y);
		mpRenderingSystem->RenderTexture(statusBarImageFontTexture, xText, yText);
		mpRenderingSystem->Render();
	}

	cleanup(wallTileTexture);
	cleanup(testSpriteSheet);
	cleanup(playerSprite);
	cleanup(background);
	Game::GetGame()->Shutdown();
	return;
}