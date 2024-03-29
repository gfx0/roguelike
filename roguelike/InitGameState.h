#ifndef InitGameState_h__
#define InitGameState_h__

#include "GameStateBase.h"

//forward declarations.
struct SDL_Texture;

class InitGameState : public GameStateBase
{
public:
	InitGameState();
	~InitGameState();

	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnUpdate(unsigned int deltaTime);
	virtual void OnInput(SDL_Event&);

protected:
private:

	//This will be false until assets etc. have been loaded.
	bool mIsNewGameLoading;

	class RenderingSystem * mpRenderingSystem;
	class ImageLoader *		mpImageLoader;
	class FontLoader *		mpFontLoader;
	class OperatingSystem * mpOperatingSystem;

	//NOTE: For now these sprites are stupidly here, later on in spritemanager / in their 
	//		own CharacterManager / Character classes.
	SDL_Texture * mpLoadingScreenBackground;
	SDL_Texture * mpLoadingScreenTextMessage;

	/**********************************
	 * Temporary "demo" player action.
	 **********************************/
	int mPlayerTempX;
	int mPlayerTempY;
	int mCachedWindowWidth;
	int mCachedWindowHeight;
	int mCachedWindowHeightHalf;
	int mCachedWindowWidthHalf;

	int mTimeToWaitUntilTransitioningMS;
};

#endif // InitGameState_h__