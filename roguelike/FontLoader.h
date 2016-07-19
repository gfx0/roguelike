#ifndef FontLoader_h__
#define FontLoader_h__
#include "EngineComponentBase.h"

struct SDL_Color;
struct SDL_Renderer;
struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

class FontLoader : public EngineComponentBase
{
public:
	FontLoader();
	~FontLoader();

public:

	enum fonts
	{
		FONT_SAMPLE = 0,
		FONT_BIG = 1,
		FONT_SMALL = 2
	};

	bool LoadFont(const char * fontPath, int fontSize);
	SDL_Texture * RenderText(char * message, fonts fontToUse , SDL_Color color, int fontSize, SDL_Renderer *renderer);
	bool Start();
	void Stop();
	void Update();


private:
	//TODO: Vectorize many fonts
	TTF_Font * mFont;
};
#endif // FontLoader_h__
