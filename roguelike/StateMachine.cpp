#include "StateMachine.h"

#include <windows.h>
#include <cassert>

#include <SDL.h> //for SDL_Event input handling

#pragma optimize("", off)

/*****************************************************************************

	Purpose of this is to control the Game logic and avoid game features 
	creeping into the engine code to bloat it.

	Separation of Game and Engine is important, so other games can be made
	with this same technology base. This component helps make that possible.

*****************************************************************************/

StateMachine::StateMachine()
	: mCurrentStateName(NULL)
{
	mpGameStateMap = new GameStateMap();
	mpPreviousState = mpGameStateMap->end();
	mpCurrentState = mpGameStateMap->end();
	mpGameStateMap->reserve(3);
}

StateMachine::~StateMachine()
{
	delete mpGameStateMap;
}

//TODO: Refactor this piece of junk, the unordered_map support has an internal visual studio bug in vs 2012 x64 bit etc... Horrible shit.
//		Go for something simpler, slower is fine too, just something that works for now, optimization can be done later.
void StateMachine::TransitionTo(const char* gameStatePath)
{
	//On the very first transition, this will be null on purpose.
	if (mCurrentStateName != NULL)
	{
		const char * tits = mCurrentStateName;
		mpPreviousState = mpGameStateMap->find(tits);
		assert(mpPreviousState != mpGameStateMap->end());
		mpPreviousState->second->OnExit();
	}

	if ( mCurrentStateName == NULL )
		mCurrentStateName = (char*)malloc(1024 * sizeof(char));
	sprintf_s(mCurrentStateName, 1024*sizeof(char), "%s", gameStatePath);

	mpCurrentState = mpGameStateMap->find(gameStatePath);
	assert(mpCurrentState != mpGameStateMap->end());
	mpCurrentState->second->OnEnter();

}

void StateMachine::ListLoadedStates()
{
	for (GameStateMap::iterator it = mpGameStateMap->begin(); it != mpGameStateMap->end(); ++it)
	{
		OutputDebugStringA("Component loaded:");
		OutputDebugStringA(it->first);
		OutputDebugStringA("\n");
	}
}

void StateMachine::CurrentStateOnUpdate()
{
	mpCurrentState->second->OnUpdate();
}

void StateMachine::CurrentStateOnInput(SDL_Event & pEvent)
{
	mpCurrentState->second->OnInput(pEvent);
}

//Meant to used with engine shutdown for automatic cleanup.
void StateMachine::CurrentStateOnExit()
{
	mpCurrentState->second->OnExit();
}