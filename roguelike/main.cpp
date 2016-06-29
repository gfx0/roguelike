#include "globals.h"

#include <windows.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <assert.h>

#include "sdlhelpers.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//NOTE: These square tiles won't scale with 16:9 etc.
const int TILE_SIZE = 40;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	/****************************************************
	* TODO:
	* 
	*	Store the results of these sub system 
	*	initializations into the game engine for generic
	*	access while progressing in the finite state
	*	machine.
	*	
	*	
	* TODO:
	* 
	*	Write cleanups for all textures separately.
	*	Perhaps a TextureManager class which stores
	*	all textures persistently in memory anyway?
	*	
	*	Order of cleanup / destructing objects is 
	*	important, don't just write one cleanup function
	*	for all types without knowing the order in which
	*	memory allocated SDL objects should be destroyed in.
	*
	* TODO:
	* 
	*	Create a TextRenderingManager or something.
	*	
	*	Purpose:
	*	
	*		1. Static texts rendered into one texture
	*		2. Bitmap fonts for fast changing texts?
	*		
	*		A combination of these two techniques should
	*		be used for efficient text rendering if
	*		realtime updating text fields are wanted in
	*		this game project. Perhaps damage / hit 
	*		indicators / floating combat text?
	*	
	*		See this for font atlas creation:
	*		http://www.angelcode.com/products/bmfont/
	*		
	*		
	* TODO:
	*		Separate texture / asset management!
	*		
	*		Separate sub system for real.
	*		
	*		Cleanup is too complex due to this now.
	*		
	****************************************************/
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
		shutdownSDL();
		return 1;
	}

	/**************************
	* Initialize Renderer
	**************************/
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		printf("CreateRenderer error: %s\n", SDL_GetError());
		cleanup(window);
		shutdownSDL();
		return 1;
	}

	/**************************
	* Initialize SDL_Image
	**************************/
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("SDL_Image IMG_Init error: %s\n", SDL_GetError());
		cleanup(renderer);
		cleanup(window);
		shutdownSDL();
		return 1;
	}

	/**************************
	* Initialize SDL_ttf
	**************************/
	if (TTF_Init() != 0) {
		printf("TTF_Init: %s\n", SDL_GetError());
		shutdownSDL();
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
	SDL_Texture *statusBarImageFontTexture = renderText("Level: 1 Int: 1 Str: 1 Dex: 1", fontPath, color, 32, renderer);
	if (statusBarImageFontTexture == nullptr) {
		cleanup(renderer);
		cleanup(window);
		shutdownSDL();
		return 1;
	}

	//Get the texture w/h so we can center it in the screen
	int iTextW, iTextH;
	SDL_QueryTexture(statusBarImageFontTexture, NULL, NULL, &iTextW, &iTextH);
	int xText = 0;//SCREEN_WIDTH / 2 - iTextW / 2;
	int yText = 0;// SCREEN_HEIGHT / 2 - iTextH / 2;

	/**************************
	* Load Textures
	**************************/
	char grassTileImagePath[1024];
	sprintf(grassTileImagePath, "%sgfx\\grasstile.png", resourcePath);
	char wallTileImagePath[1024];
	sprintf(wallTileImagePath, "%sgfx\\walltile.png", resourcePath);
	char foregroundImagePath[1024];
	sprintf(foregroundImagePath, "%sgfx\\knight.png", resourcePath);
	char spriteSheetTestPath[1024];
	sprintf(spriteSheetTestPath, "%sgfx\\testspritesheet.png", resourcePath);

	SDL_Texture *background = loadTexture(grassTileImagePath, renderer);
	SDL_Texture *wallTileTexture = loadTexture(wallTileImagePath, renderer);
	SDL_Texture *playerSprite = loadTexture(foregroundImagePath, renderer);
	SDL_Texture *testSpriteSheet = loadTexture(spriteSheetTestPath, renderer);

	if (background == NULL|| playerSprite == NULL || wallTileTexture == NULL || testSpriteSheet == NULL ) {
		cleanup(testSpriteSheet);
		cleanup(wallTileTexture);
		cleanup(background);
		cleanup(playerSprite);
		cleanup(renderer);
		cleanup(window);
		shutdownSDL();
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
		renderTexture(wallTileTexture, renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		renderTexture(playerSprite, renderer, x, y);
		renderTexture(statusBarImageFontTexture, renderer, xText, yText);
		SDL_RenderPresent(renderer);
	}

	cleanup(wallTileTexture);
	cleanup(testSpriteSheet);
	cleanup(playerSprite);
	cleanup(background);
	cleanup(renderer);
	cleanup(window);
	shutdownSDL();
	return 0;
}