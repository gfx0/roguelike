#ifndef ImageLoader_h__
#define ImageLoader_h__
#include "EngineComponentBase.h"

struct SDL_Texture;
struct SDL_Renderer;

class ImageLoader : public EngineComponentBase
{
public:
	ImageLoader();
	~ImageLoader();

private:

public:
	bool Start();
	void Stop();
	void Update();
	SDL_Texture* LoadTexture(char * filePath, SDL_Renderer *ren);
};
#endif // ImageLoader_h__
