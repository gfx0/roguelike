#include "RenderingSystem.h"

#include <stdio.h>
#include <SDL.h>
#include <cassert>

/*****************************************************************************

	Purpose of this system is to display graphics on screen for the player.

*****************************************************************************/

RenderingSystem::RenderingSystem()
: mpWindow(NULL)
, mpRenderer(NULL)
{

}

RenderingSystem::~RenderingSystem()
{

}

void RenderingSystem::Render()
{
	SDL_RenderPresent(mpRenderer);
}

void RenderingSystem::Clear()
{
	SDL_RenderClear(mpRenderer);
}

bool RenderingSystem::Start()
{
	return true;
}


void RenderingSystem::Stop()
{
	SDL_DestroyRenderer(mpRenderer);
	SDL_DestroyWindow(mpWindow);
}


void RenderingSystem::Update( unsigned int deltaTime )
{

}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void RenderingSystem::RenderTexture(SDL_Texture *tex, int x, int y, int w, int h)
{
	assert(mpRenderer);
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(mpRenderer, tex, NULL, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/

void RenderingSystem::RenderTexture(SDL_Texture *tex, int x, int y)
{
	assert(mpRenderer);
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	RenderTexture(tex, x, y, w, h);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void RenderingSystem::RenderTexture(SDL_Texture *tex, SDL_Rect dst,	SDL_Rect *clip = nullptr)
{
	assert(mpRenderer);
	SDL_RenderCopy(mpRenderer, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void RenderingSystem::RenderTexture(SDL_Texture *tex, int x, int y,	SDL_Rect *clip = nullptr)
{
	assert(mpRenderer);
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	RenderTexture(tex, dst, clip);
}

int RenderingSystem::CreateWindow(WindowCreationInfo &info)
{
	/**************************
	* Initialize Window
	**************************/
	int fullscreenFlag = info.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
	int borderFlag = info.borderless ? SDL_WINDOW_BORDERLESS : 0;
	mpWindow = SDL_CreateWindow("Roguelike", info.x, info.y, info.w, info.h, SDL_WINDOW_SHOWN | borderFlag | fullscreenFlag);
	if (mpWindow == nullptr) {
		SDL_Log("CreateWindow error: %s\n", SDL_GetError());
		return 2;
	}

	/**************************
	* Initialize Renderer
	**************************/
	mpRenderer = SDL_CreateRenderer(mpWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mpRenderer == nullptr) {
		SDL_Log("CreateRenderer error: %s\n", SDL_GetError());
		SDL_DestroyWindow(mpWindow);
		return 3;
	}

	return 0;
}

int RenderingSystem::GetWindowWidth()
{
	assert(mpWindow);
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_GetWindowSize(mpWindow, &windowWidth, &windowHeight);
	return windowWidth;
}

int RenderingSystem::GetWindowHeight()
{
	assert(mpWindow);
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_GetWindowSize(mpWindow, &windowWidth, &windowHeight);
	return windowHeight;
}

SDL_Window * RenderingSystem::getWindow()
{
	assert(mpWindow);
	return mpWindow;
}

SDL_Renderer * RenderingSystem::GetRenderer()
{
	assert(mpRenderer);
	return mpRenderer;
}
