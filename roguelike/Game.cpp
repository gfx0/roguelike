#include "Game.h"

/*****************************************************************************

	This is the heart of the game engine, keep this clean please.

	Every EngineComponent and GameState first appears here.

*****************************************************************************/

// Game States
#include "InitGameState.h"
#include "MainMenuState.h"
#include "PlayGameState.h"

// Game Components
#include "InputManager.h"
#include "RenderingSystem.h"
#include "ImageLoader.h"
#include "FontLoader.h"
#include "OperatingSystem.h"

// Engine Core
#include "EngineComponentManager.h"
#include "StateMachine.h"


// For proper order SDL Shutdown
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

bool Game::instanceOfGameExists = false;
Game* Game::pGame = NULL;

Game::Game()
	: mIsInitializing(false)
	, mIsRunning(false)
	/**************
	 * Time Init.
	 **************/
	, mStartTime(0)
	, mTotalTime(0)
	, mCurrentTime(0)
	, mLastTime(0)
	, mDeltaTime(0)
{
	mpStateMachine = new StateMachine();
	mpEngineComponentManager = new EngineComponentManager();
	pGame = this;
}

Game::~Game()
{
	delete mpStateMachine;
	delete mpEngineComponentManager;
}

Game* Game::GetGame()
{
	assert(pGame);
	return pGame;
}

void Game::Start()
{
	mIsInitializing = true;
	
	/*************************************************************************
	 *	Load Game States
	 *************************************************************************/
	mpStateMachine->LoadState<InitGameState>(GAMESTATE_INIT);
	mpStateMachine->LoadState<MainMenuState>(GAMESTATE_MAINMENU);
	mpStateMachine->LoadState<PlayGameState>(GAMESTATE_PLAYGAME);

	/*************************************************************************
	 *	Load Engine Game Components
	 *************************************************************************/
	mpEngineComponentManager->LoadComponent<OperatingSystem>("OperatingSystem");
	mpEngineComponentManager->LoadComponent<RenderingSystem>("RenderingSystem");
	mpEngineComponentManager->LoadComponent<ImageLoader>("ImageLoader");
	mpEngineComponentManager->LoadComponent<FontLoader>("FontLoader");
	mpEngineComponentManager->LoadComponent<InputManager>("InputManager");
	
	//mpEngineComponentManager->ListLoadedComponents();

	/*************************************************************************
	*	If even one component fails to initialize, shut down engine and fix.
	*************************************************************************/
	if (!mpEngineComponentManager->InitializeAllComponents())
		return this->Shutdown("Some engine component returned false when trying to start it! Something is borked!");

	/*************************************************************************
	*	Initialization successful, do post init stuff.
	*************************************************************************/
	mIsRunning = true;
	mIsInitializing = false;


	/*************************************************************************
	*	Initialize game core timer.
	*************************************************************************/
	mStartTime = SDL_GetTicks();
	mTotalTime = mStartTime;
	mLastTime = 0;
	mDeltaTime = 0;

	/*************************************************************************
	*	Start running meaningful gameplay.
	*************************************************************************/
	mpStateMachine->TransitionTo(GAMESTATE_INIT);

}

bool Game::IsOn()
{
	return mIsRunning || mIsInitializing;
}

void Game::Update()
{

	if (mIsInitializing)
		return;

	mCurrentTime = SDL_GetTicks();
	mTotalTime += mCurrentTime;
	mDeltaTime = mCurrentTime - mLastTime;

	//Debug timers:
	//SDL_Log("curTime: %d totalTime: %d lastTime: %d deltaTime: %f test: %fs\n", mCurrentTime, mTotalTime, mLastTime, mDeltaTime);

	mLastTime = mCurrentTime;

	/**********************************
	*	Poll for input.
	***********************************/
	SDL_Event mpSDLEventInQueue;
	if (SDL_PollEvent(&mpSDLEventInQueue) == 1) //Returns 1 if event is pending.
		mpStateMachine->CurrentStateOnInput(mpSDLEventInQueue);

	/**********************************
	*	Update engine components.
	***********************************/
	mpEngineComponentManager->Update( mDeltaTime );

	/**********************************
	*	Update current game state.
	***********************************/
	mpStateMachine->CurrentStateOnUpdate( mDeltaTime );

	//Render here or in state?
	//mpRenderingSystem->render();

}

void Game::Shutdown( char * reasonForShutdown )
{

	if ( mpStateMachine )
		mpStateMachine->CurrentStateOnExit();

	if ( reasonForShutdown != nullptr )
		SDL_Log("\n\n\nSHUTDOWN -> REASON: %s\n\n\n", reasonForShutdown);

	mpEngineComponentManager->DeInitializeAllComponents();

	//NOTE: SDL and related subsystems are killed here.
	//		Engine components kill their own used SDL resources by themselves.
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	mIsRunning = false;
}

