#ifndef RenderingSystem_h__
#define RenderingSystem_h__
#include "EngineComponentBase.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct WindowCreationInfo
{
	WindowCreationInfo()
		: borderless(false)
		, fullscreen(false)
	{}

	int x;
	int y; 
	int w; 
	int h; 
	bool borderless;
	bool fullscreen;
};

class RenderingSystem : public EngineComponentBase
{
public:
	RenderingSystem();
	~RenderingSystem();

	bool Start();
	void Stop();
	void Update(unsigned int deltaTime);

	void Render();
	void Clear();

	void RenderTexture(SDL_Texture * tex, int x, int y, int w, int h);
	void RenderTexture(SDL_Texture * tex, int x, int y);
	void RenderTexture(SDL_Texture * tex, SDL_Rect dst, SDL_Rect * clip);
	void RenderTexture(SDL_Texture * tex, int x, int y, SDL_Rect * clip);

	int CreateWindow(WindowCreationInfo &info);
	int GetWindowWidth();
	int GetWindowHeight();
	SDL_Window * getWindow();

	SDL_Renderer * GetRenderer();
private:
	SDL_Window * mpWindow;
	SDL_Renderer * mpRenderer;
};
#endif // RenderingSystem_h__
