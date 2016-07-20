#ifndef GameStateBase_h__
#define GameStateBase_h__

union SDL_Event;

class GameStateBase
{
public:
	GameStateBase();
	~GameStateBase();

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnInput(SDL_Event&) = 0;
protected:
private:
};

#endif // GameStateBase_h__
