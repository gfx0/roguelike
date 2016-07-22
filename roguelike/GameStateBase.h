#ifndef GameStateBase_h__
#define GameStateBase_h__

union SDL_Event;

/******************************************************************************
 *
 * NOTE: This IDS_OF_GAME_STATES enumerator is here for convenience. 
 *
 *		 This way game states can use transitions without an extra include.
 *		 
 *****************************************************************************/
enum IDS_OF_GAME_STATES
{
	GAMESTATE_INIT = 0,
	GAMESTATE_MAINMENU,
	GAMESTATE_PLAYGAME,
	GAMESTATE_MAX
};

class GameStateBase
{
public:
	GameStateBase();
	~GameStateBase();

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void OnUpdate(unsigned int deltaTime) = 0;
	virtual void OnInput(SDL_Event&) = 0;
protected:
private:
};

#endif // GameStateBase_h__
