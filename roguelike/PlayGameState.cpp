#include "PlayGameState.h"
#include "settings.h"
#include "Game.h"
/*
#include "OldRenderingSystem.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "SpriteManager.h"
#include "ProjectileManager.h"
*/
////#include <windows.h>
////#include <time.h> //srand
////
////#include <stdio.h>
////#include <SDL.h>
////#include <SDL_image.h>
////#include <SDL_ttf.h>
////#include <assert.h>
////
////
//#include "sdlhelpers.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
PlayGameState::PlayGameState()
	: mIsNewGameLoading(false)
	, mIsNewGameSessionInitialized(false)
{

}

PlayGameState::~PlayGameState()
{

}

void PlayGameState::OnEnter()
{

	//HackySDLGameStart();
	/*
	mIsNewGameSessionInitialized = false;
	mIsNewGameLoading = false;
	mpOldRenderingSystem = Game::GetGame()->GetGameComponentManager()->GetComponent<RenderingSystem>("OldRendreingComponent");
	mpPlayerManager = Game::GetGame()->GetGameComponentManager()->GetComponent<PlayerManager>("PlayerManager");
	mpSpriteManager = Game::GetGame()->GetGameComponentManager()->GetComponent<SpriteManager>("SpriteManager");
	mpEnemyManager = Game::GetGame()->GetGameComponentManager()->GetComponent<EnemyManager>("EnemyManager");
	mpProjectileManager = Game::GetGame()->GetGameComponentManager()->GetComponent<ProjectileManager>("ProjectileManager");

	mpPlayerManager->GetPlayer(0)->SetX(SETTINGS_PLAYER_START_X);
	mpPlayerManager->GetPlayer(0)->SetY(SETTINGS_PLAYER_START_Y);

	//NOTE: OLD! (2013)
	if (!mpOldRenderingSystem->IsInitialized())
	{
		mpOldRenderingSystem->InitWindow(SETTINGS_SCREEN_WIDTH, SETTINGS_SCREEN_HEIGHT);
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

void PlayGameState::OnExit()
{
	//delete mpOldRenderingSystem; ?
}

void PlayGameState::OnUpdate()
{
	/*
	if (mIsNewGameSessionInitialized && !mIsNewGameLoading)
	{
		mIsNewGameLoading = true;
		LoadNewLevelAndStartGame();
	}

	mpOldRenderingSystem->DrawSomeText(240, 200, "Loading new game...");
	*/
}

void PlayGameState::LoadNewLevelAndStartGame()
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