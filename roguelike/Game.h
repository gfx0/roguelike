/********************************************************************

created:	9.7.2016
author:		Jani Mäkinen
purpose:	All components this game uses should be located here.

*********************************************************************/

#ifndef Game_h__
#define Game_h__

#include "StateMachine.h"
#include "EngineComponentManager.h"

#include <cassert>

class Game
{
public:

	//NOTE: Should this be enabled so it can be fetched in any state?
	static Game* GetGame();
	Game();
	~Game();
	void Start();
	void Shutdown(char * reason = nullptr);
	void Update();
	bool IsOn();

	class StateMachine* GetStateMachine() { return mpStateMachine; };
	class EngineComponentManager* GetEngineComponentManager() { return mpEngineComponentManager; };

	template<class T>
	T * GetEngineComponent(const char * gameComponentPath)
	{
		assert(mpEngineComponentManager);
		return mpEngineComponentManager->GetComponent<T>(gameComponentPath);
	}

protected:
private:
	static bool instanceOfGameExists;
	static Game * pGame;

	bool mIsRunning;
	bool mIsInitializing;

	class StateMachine *			mpStateMachine;
	class EngineComponentManager *	mpEngineComponentManager;
	
	//TODO: mpInputManager?

	/**********************************
	*	Game time.
	***********************************/
	unsigned int mStartTime;
	unsigned int mTotalTime;
	unsigned int mCurrentTime;
	unsigned int mLastTime;
	unsigned int mDeltaTime;

};

#endif // Game_h__