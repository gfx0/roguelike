#ifndef GameStateBase_h__
#define GameStateBase_h__

class GameStateBase
{
public:
	GameStateBase();
	~GameStateBase();

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void OnUpdate() = 0;
protected:
private:
};

#endif // GameStateBase_h__
