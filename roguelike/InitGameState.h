#ifndef InitGameState_h__
#define InitGameState_h__

#include "GameStateBase.h"

class InitGameState : public GameStateBase
{
public:
	InitGameState();
	~InitGameState();

	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnUpdate();

	void HackySDLGameStart();

	void LoadNewLevelAndStartGame();

protected:
private:

	bool mIsNewGameLoading;
	bool mIsNewGameSessionInitialized;

	class RenderingSystem * mpRenderingSystem;
	class ImageLoader * mpImageLoader;
	class FontLoader * mpFontLoader;
	class OperatingSystem * mpOperatingSystem;
	/*
	class RenderingSystem *			mpOldRenderingSystem;
	class PlayerManager *		mpPlayerManager;
	class SpriteManager *		mpSpriteManager;
	class EnemyManager *		mpEnemyManager;
	class ProjectileManager *	mpProjectileManager;
	*/

};

#endif // InitGameState_h__