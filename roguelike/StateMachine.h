#ifndef StateMachine_h__
#define StateMachine_h__

#include "GameStateBase.h"

#include <unordered_map>

union SDL_Event;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void TransitionTo(const char* stateName);
	void CurrentStateOnUpdate();
	void CurrentStateOnInput(SDL_Event & pEvent);
	void CurrentStateOnExit();
	void ListLoadedStates();

	template<class T>
	void LoadState(const char * gameStatePath)
	{
		//TODO: prevent the creation of a new instance of an already existing engine component!
		//if ( mpGameStateMap->find(componentPath) != mEngineComponents->end() )
		T * GameState = new T();
		mpGameStateMap->insert(std::make_pair(gameStatePath, GameState));
	}

	template<class T>
	T * GetState(const char * gameStatePath)
	{
		GameStateMap::iterator itr = mpGameStateMap->find(gameStatePath);
		assert(itr == mpGameStateMap->end());

		return static_cast<T*>(itr->second);
	}

private:
	typedef std::tr1::unordered_map<const char*, GameStateBase*> GameStateMap;
	GameStateMap * mpGameStateMap;
	GameStateMap::iterator mpPreviousState;
	GameStateMap::iterator mpCurrentState;
	char * mCurrentStateName;
};

#endif // StateMachine_h__