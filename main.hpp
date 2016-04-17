#ifndef MAIN_HPP
#define MAIN_HPP
#include <string>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <windows.h>
#include <math.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* gFont;

extern Mix_Music* gMusic;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

#endif
