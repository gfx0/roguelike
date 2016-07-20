#include "StateMachine.h"

#include <windows.h>
#include <cassert>

#include <SDL.h> //for SDL_Event input handling

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
}

StateMachine::~StateMachine()
{
	delete mpGameStateMap;
}

void StateMachine::TransitionTo(char* gameStatePath)
{
	//On the very first transition, this will be null on purpose.
	if (mCurrentStateName != NULL)
	{
		mpPreviousState = mpGameStateMap->find(mCurrentStateName);
		assert(mpPreviousState != mpGameStateMap->end());
		mpPreviousState->second->OnExit();
	}

	mCurrentStateName = gameStatePath;

	mpCurrentState = mpGameStateMap->find(mCurrentStateName);
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
