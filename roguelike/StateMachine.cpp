#include "StateMachine.h"

#include <SDL.h> //for SDL_Event input handling

/*****************************************************************************

Purpose of this is to control the Game logic and avoid game features
creeping into the engine code to bloat it.

Separation of Game and Engine is important, so other games can be made
with this same technology base. This component helps make that possible.

*****************************************************************************/

StateMachine::StateMachine()
	: mCurrentGameStateID(0)
{
	mpGameStates.reserve(GAMESTATE_MAX);
	for ( unsigned int i = 0; i < GAMESTATE_MAX; i++ )
		mpGameStates.push_back(NULL);
	mpCurrentState = mpPreviousState = NULL;
}

StateMachine::~StateMachine()
{
	//NOTE: This deletes the actual pointers to the new'd gamestate pointer objects.
	//		Using boost::ptr_vector could be better or other more advanced pointers but thats not the goal here.
	for (unsigned int i = 0; i < mpGameStates.size(); i++)
		delete mpGameStates[i];

	mpGameStates.clear();
}

void StateMachine::TransitionTo(int stateID)
{
	assert(stateID < mpGameStates.size());
	assert(mpGameStates[stateID] != NULL);

	mCurrentGameStateID = stateID;

	//First transition can be detected from both state pointers being null.
	if (mpCurrentState == NULL && mpPreviousState == NULL)
	{
		mpCurrentState = mpGameStates[stateID];
		mpCurrentState->OnEnter();
		return;
	}

	mpPreviousState = mpCurrentState;
	mpCurrentState = mpGameStates[stateID];
	mpPreviousState->OnExit();
	mpCurrentState->OnEnter();
}

void StateMachine::ListLoadedStates()
{
	for (unsigned int i = 0; i < mpGameStates.size(); i++)
	{
		if (mpGameStates[i] != NULL)
			SDL_Log("State loaded: %d\n", i);
		else
			SDL_Log("State NULL/Empty: %d\n", i);
	}
}

GameStateBase * StateMachine::GetState(int gameStateID)
{
	assert(gameStateID < mpGameStates.size());
	assert(mpGameStates[gameStateID] != NULL);

	return mpGameStates[gameStateID];
}

int StateMachine::GetCurrentGameStateID()
{
	return mCurrentGameStateID;
}

void StateMachine::CurrentStateOnUpdate( unsigned int deltaTime )
{
	mpCurrentState->OnUpdate( deltaTime );
}

void StateMachine::CurrentStateOnInput(SDL_Event & pEvent)
{
	mpCurrentState->OnInput(pEvent);
}

void StateMachine::CurrentStateOnExit()
{
	mpCurrentState->OnExit();
}