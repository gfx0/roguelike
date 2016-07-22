#ifndef PlayGameState_h__
#define PlayGameState_h__

#include "GameStateBase.h"

//forward declarations.
struct SDL_Texture;

class PlayGameState : public GameStateBase
{
public:
	PlayGameState();
	~PlayGameState();

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
	SDL_Texture * mpWallTileTexture;
	SDL_Texture * mpPlayerSprite;
	SDL_Texture * mpTestSpriteSheet;
	SDL_Texture * mpStatusBarImageFontTexture;

	/**********************************
	 * Temporary "demo" player action.
	 **********************************/
	int mPlayerTempX;
	int mPlayerTempY;
	int mCachedWindowWidth;
	int mCachedWindowHeight;
	int mCachedWindowHeightHalf;
	int mCachedWindowWidthHalf;
};

#endif // PlayGameState_h__