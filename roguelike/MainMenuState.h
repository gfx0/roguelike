#ifndef MainMenuState_h__
#define MainMenuState_h__

#include "GameStateBase.h"

//forward declarations.
struct SDL_Texture;

class MainMenuState : public GameStateBase
{
public:
	MainMenuState();
	~MainMenuState();

	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnUpdate();
	virtual void OnInput(SDL_Event&);

protected:
private:

	class RenderingSystem * mpRenderingSystem;
	class ImageLoader *		mpImageLoader;
	class FontLoader *		mpFontLoader;
	class OperatingSystem * mpOperatingSystem;

	//NOTE: For now these sprites are stupidly here, later on in spritemanager / in their 
	//		own CharacterManager / Character classes.
	SDL_Texture * mpMainMenuBackgroundImage;
	SDL_Texture * mpGameTitle;
	SDL_Texture * mpMainMenuOptionsTexts;

	int mGameTitleX;
	int mGameTitleY;
	int mMainMenuTextsX;
	int mMainMenuTextsY;
};

#endif // MainMenuState_h__