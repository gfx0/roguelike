#ifndef StateMachine_h__
#define StateMachine_h__

#include "GameStateBase.h"

#include <vector>
#include <assert.h>

union SDL_Event;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void TransitionTo(int stateID);
	void CurrentStateOnUpdate(unsigned int deltaTime);
	void CurrentStateOnInput(SDL_Event & pEvent);
	void CurrentStateOnExit();
	void ListLoadedStates();

	template<class T>
	void LoadState(int indexPosition)
	{
		assert(indexPosition < mpGameStates.size());
		//TODO: prevent the creation of a new instance of an already existing engine component!
		//if ( mpGameStateMap->find(componentPath) != mEngineComponents->end() )
		T * pNewGameState = new T();
		assert(mpGameStates[indexPosition] == NULL);
		mpGameStates[indexPosition] = pNewGameState;
	}

	GameStateBase* GetState(int gameStateID);
	int GetCurrentGameStateID();

private:
	std::vector<GameStateBase*> mpGameStates;
	GameStateBase* mpPreviousState;
	GameStateBase* mpCurrentState;

	int mCurrentGameStateID;

};

#endif // StateMachine_h__