#include "FontLoader.h"

#include "settings.h"

#include <SDL_ttf.h>
#include <stdio.h>

/*****************************************************************************

	Purpose of this component is to purely load many types truetype fonts.
	It could contain the fonts themselves, since the engine might get 
	"manager bloat" otherwise? hmm..
	
	TODO: Add support for bitmap fonts?

*****************************************************************************/

FontLoader::FontLoader()
{

}

FontLoader::~FontLoader()
{

}

bool FontLoader::Start()
{
	if ( TTF_Init() != 0 )
	{
		printf("TTF_Init: %s\n", SDL_GetError());
		return false;
	}
	return true;
}


void FontLoader::Stop()
{
	if ( mFont )
		TTF_CloseFont(mFont);
}


void FontLoader::Update()
{

}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture * FontLoader::RenderText(char * message, fonts fontToUse, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//TODO: Add support for more fonts...
	if (fontToUse != FONT_SAMPLE)
		return nullptr;

	SDL_Surface *surf = TTF_RenderText_Blended(mFont, message, color);
	if (surf == nullptr) {
		TTF_CloseFont(mFont);
		printf("TTF_RenderText %s\n", SDL_GetError());
		return nullptr;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		printf("CreateTexture %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surf);

	return texture;
}

bool FontLoader::LoadFont(const char * fullFontFilePath, int fontSize)
{
	//TODO: Save the mFont into a vector std::vector<Font> mFonts;
	//
	//		Also save font size 
	//		Perhaps color too?
	//

	mFont = TTF_OpenFont(fullFontFilePath, fontSize);
	if (mFont == nullptr) {
		printf("TTF_OpenFont error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}