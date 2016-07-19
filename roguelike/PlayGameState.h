#ifndef PlayGameState_h__
#define PlayGameState_h__

#include "GameStateBase.h"

class PlayGameState : public GameStateBase
{
public:
	PlayGameState();
	~PlayGameState();

	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnUpdate();

	void LoadNewLevelAndStartGame();

protected:
private:

	bool mIsNewGameLoading;
	bool mIsNewGameSessionInitialized;

	/*
	class RenderingSystem *		mpOldRenderingSystem;
	class PlayerManager *		mpPlayerManager;
	class SpriteManager *		mpSpriteManager;
	class EnemyManager *		mpEnemyManager;
	class ProjectileManager *	mpProjectileManager;
	*/

};

#endif // PlayGameState_h__