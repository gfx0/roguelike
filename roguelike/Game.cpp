#include "Game.h"

/*****************************************************************************

	This is the heart of the game engine, keep this clean please.

	Every EngineComponent and GameState first appears here.

*****************************************************************************/

// Game States
#include "InitGameState.h"
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
	/*
	
	Singleton pattern enforcement, I think this is not useful, todo: remove.

	if (!instanceOfGameExists)
	{
		instanceOfGameExists = true;
		pGame = new Game();
		return pGame;
	}
	else
	*/
	return pGame;

}

void Game::Start()
{
	mIsInitializing = true;
	
	/*************************************************************************
	 *	Load Game States
	 *************************************************************************/
	mpStateMachine->LoadState<InitGameState>("InitializeTheGame");
	mpStateMachine->LoadState<PlayGameState>("PlayGameState");

	/*************************************************************************
	 *	Load Engine Game Components
	 *************************************************************************/
	mpEngineComponentManager->LoadComponent<OperatingSystem>("OperatingSystem");
	mpEngineComponentManager->LoadComponent<RenderingSystem>("RenderingSystem");
	mpEngineComponentManager->LoadComponent<ImageLoader>("ImageLoader");
	mpEngineComponentManager->LoadComponent<FontLoader>("FontLoader");
	mpEngineComponentManager->LoadComponent<InputManager>("InputManager");
	
	mpEngineComponentManager->ListLoadedComponents();

	/*************************************************************************
	*	If even one component fails to initialize, shut down engine and fix.
	*************************************************************************/
	if (!mpEngineComponentManager->InitializeAllComponents())
		this->Shutdown();

	/*************************************************************************
	*	Initialization successful, do post init stuff.
	*************************************************************************/
	mIsRunning = true;
	mIsInitializing = false;

	mpStateMachine->TransitionTo("InitializeTheGame");

}

bool Game::IsOn()
{
	return mIsRunning || mIsInitializing;
}

void Game::Update()
{

	if (mIsInitializing)
		return;

	//Poll for input

	//Update all game components
	mpEngineComponentManager->Update();

	//Update current game state
	mpStateMachine->CurrentStateOnUpdate();

	//Render here or in state?
	//mpRenderingSystem->render();

}

void Game::Shutdown( char * reasonForShutdown )
{

	if ( reasonForShutdown != nullptr )
		printf(reasonForShutdown);

	mpEngineComponentManager->DeInitializeAllComponents();

	//NOTE: SDL and related subsystems are killed here.
	//		Engine components kill their own used SDL resources by themselves.
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	mIsRunning = false;
}

