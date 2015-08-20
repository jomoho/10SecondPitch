//============================================================================
// Name        : 10SecondPitch.cpp
// Author      : Moritz Laass
// Version     :
// Copyright   : Free for all #LD48 number 27, August 2013
//============================================================================

#include <iostream>
#include "RendererSDL.h"
#include "App.h"
#include "Text.h"

using namespace std;

int main(int argc, char** argv) {
	App *app = App::get();

	app->start();
	while(!app->done)  // Enter main loop.
	{
		app->run();
	 }

	App::destroyInstance();
	return 0;
}
