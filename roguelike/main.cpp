#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <assert.h>

#include "sdlhelpers.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//We'll just be using square tiles for now
const int TILE_SIZE = 40;



/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(char * message, char * fontFilePath,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFilePath, fontSize);
	if (font == nullptr) {
		printf("TTF_OpenFont error: %s\n", SDL_GetError());
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		printf("TTF_RenderText %s\n", SDL_GetError());
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		printf("CreateTexture %s\n", SDL_GetError());
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}




/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(char * filePath, SDL_Renderer *ren)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, filePath);
	if (texture == nullptr) {
		printf("LoadTexture error: %s\n", SDL_GetError());
	}
	return texture;
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
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
/*
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
int w, h;
SDL_QueryTexture(tex, NULL, NULL, &w, &h);
renderTexture(tex, ren, x, y, w, h);
}
*/

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
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
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr)
{
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
	renderTexture(tex, ren, dst, clip);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	/**************************
	* Initialize Window
	**************************/
	SDL_Window *window = SDL_CreateWindow("Roguelike", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		printf("CreateWindow error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	/**************************
	* Initialize Renderer
	**************************/
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		printf("CreateRenderer error: %s\n", SDL_GetError());
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	/**************************
	* Initialize SDL_Image
	**************************/
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("SDL_Image IMG_Init error: %s\n", SDL_GetError());
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	/**************************
	* Initialize SDL_ttf
	**************************/
	if (TTF_Init() != 0) {
		printf("TTF_Init: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	/**************************
	* Get SDL Resource Path
	**************************/

	char *resourcePath = SDL_GetBasePath();

	/**************************
	* Load Font & Render it
	**************************/
	char fontPath[1024];
	sprintf(fontPath, "%sgfx\\sample.ttf", resourcePath);
	//We'll render the string "TTF fonts are cool!" in white
	//Color is in RGBA format
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture *fontImage = renderText("Level: 1 Int: 1 Str: 1 Dex: 1", fontPath, color, 32, renderer);
	if (fontImage == nullptr) {
		cleanup(renderer);
		cleanup(window);
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	//Get the texture w/h so we can center it in the screen
	int iTextW, iTextH;
	SDL_QueryTexture(fontImage, NULL, NULL, &iTextW, &iTextH);
	int xText = 0;//SCREEN_WIDTH / 2 - iTextW / 2;
	int yText = 0;// SCREEN_HEIGHT / 2 - iTextH / 2;

	/**************************
	* Load Textures
	**************************/
	char backgroundImagePath[1024];
	sprintf(backgroundImagePath, "%sgfx\\grasstile.png", resourcePath);
	char foregroundImagePath[1024];
	sprintf(foregroundImagePath, "%sgfx\\knight.png", resourcePath);
	char spriteSheetTestPath[1024];
	sprintf(spriteSheetTestPath, "%sgfx\\testspritesheet.png", resourcePath);

	SDL_Texture *background = loadTexture(backgroundImagePath, renderer);
	SDL_Texture *playerSprite = loadTexture(foregroundImagePath, renderer);
	SDL_Texture *testSpriteSheet = loadTexture(spriteSheetTestPath, renderer);

	if (background == nullptr || playerSprite == nullptr) {
		cleanup(background);
		cleanup(playerSprite);
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	SDL_RenderClear(renderer);

	/**************************
	* Player Position & Size
	**************************/
	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;

	int iW, iH;
	SDL_QueryTexture(playerSprite, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	/**************************
	* Sprite Sheet Pos ^ Size
	**************************/
	//iW and iH are the clip width and height
	//We'll be drawing only clips so get a center position for the w/h of a clip
	int iiW = 100, iiH = 100;
	int xx = SCREEN_WIDTH / 2 - iiW / 2;
	int yy = SCREEN_HEIGHT / 2 - iiH / 2;

	//Setup the clips for our image
	SDL_Rect clips[4];
	for (int i = 0; i < 4; ++i) {
		clips[i].x = i / 2 * iiW;
		clips[i].y = i % 2 * iiH;
		clips[i].w = iiW;
		clips[i].h = iiH;
	}
	//Specify a default clip to start with
	int useClip = 0;

	/**************************
	* SDL Events Loop
	**************************/
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
					x -= 10;
					break;
				case SDLK_RIGHT:
					x += 10;
					break;
				case SDLK_UP:
					y -= 10;
					break;
				case SDLK_DOWN:
					y += 10;
					break;
				case SDLK_1:
					useClip = 0;
					break;
				case SDLK_2:
					useClip = 1;
					break;
				case SDLK_3:
					useClip = 2;
					break;
				case SDLK_4:
					useClip = 3;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				quit = true;
			}
		}
		//Render the scene
		SDL_RenderClear(renderer);
		//Draw the tiles by calculating their positions
		for (int i = 0; i < xTiles * yTiles; ++i) {
			int x = i % xTiles;
			int y = i / xTiles;
			renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
				TILE_SIZE);
		}
		renderTexture(testSpriteSheet, renderer, xx, yy, &clips[useClip]);
		renderTexture(playerSprite, renderer, x, y);
		renderTexture(fontImage, renderer, xText, yText);
		SDL_RenderPresent(renderer);
	}

	cleanup(testSpriteSheet);
	cleanup(playerSprite);
	cleanup(background);
	cleanup(renderer);
	cleanup(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
