/*
 * Renderer.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#include "RendererSDL.h"
#include <iostream>

RendererSDL::RendererSDL(const string& title, Uint32 width, Uint32 height, Uint32 fps, bool fullscreen) {
	frames = 0;
    this->width = width;
    this->height = height;
    this->title = string(title);
    this->fps = fps;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
    	FILE_LOG(logWARNING) << "SDL_Init Error: " << SDL_GetError() << endl;
    }

    int windowflags = SDL_WINDOW_SHOWN;
    if(fullscreen){
        windowflags = windowflags|SDL_WINDOW_FULLSCREEN;
    }

    // Create a window
	window = SDL_CreateWindow(
	title.c_str() ,
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	width, height,
	windowflags);

	if (window == NULL){
		FILE_LOG(logWARNING) << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
	}

	renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL){
		FILE_LOG(logWARNING) << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
	}
	last_ticks = SDL_GetTicks();
	tick_interval = 1000/fps;
	tick_diff = tick_interval;
}

RendererSDL::~RendererSDL() {

    // Done! Close the window, clean-up and exit the program.
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

const string& RendererSDL::getTitle() {
	return title;
}

void RendererSDL::setTitle(const string& title) {
	this->title = title;
}

void RendererSDL::startFrame() {
	SDL_RenderClear(renderer);
}

void RendererSDL::endFrame() {
	SDL_RenderPresent(renderer);

	tick_diff = (int) SDL_GetTicks() - last_ticks;

	if(tick_diff > 0){
		SDL_Delay(tick_diff);
	}
	frames += 1;
	last_ticks = SDL_GetTicks();
}
