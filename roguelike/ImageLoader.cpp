#include "ImageLoader.h"

//#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>

/*****************************************************************************

	Purpose of this component is to purely load many types of images into
	SDL compatible textures, this should be a simple class without containing
	loaded images, just the loader that returns the successfull loaded pointer.

	TODO: Add JPG support for minimal size images? atm. supports only PNG

*****************************************************************************/

ImageLoader::ImageLoader()
{

}

ImageLoader::~ImageLoader()
{

}

bool ImageLoader::Start()
{
	if ( (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG )
	{
		SDL_Log("SDL_Image IMG_Init error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}


void ImageLoader::Stop()
{

}


void ImageLoader::Update()
{

}

/**
* Loads an image into a texture on the rendering device
* @param file The PNG or BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* ImageLoader::LoadTexture(char * filePath, SDL_Renderer *ren)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, filePath);
	if (texture == nullptr) {
		SDL_Log("LoadTexture error: %s\n", SDL_GetError());
	}
	return texture;
}