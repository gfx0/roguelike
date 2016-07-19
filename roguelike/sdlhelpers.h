#ifndef sdlhelpers_h
#define sdlhelpers_h

/****************************************
 *
 * SDL Cleanup helpers
 *
 ****************************************/
void cleanup(SDL_Window *win)
{
	assert(win != NULL);
	SDL_DestroyWindow(win);
}
void cleanup(SDL_Renderer *ren)
{
	assert(ren != NULL);
	SDL_DestroyRenderer(ren);
}
void cleanup(SDL_Texture *tex)
{
	assert(tex != NULL);
	SDL_DestroyTexture(tex);
}
void cleanup(SDL_Surface *sur)
{
	assert(sur != NULL);
	SDL_FreeSurface(sur);
}

#endif // !sdlhelpers_h
