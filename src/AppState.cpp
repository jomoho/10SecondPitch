/*
 * AppState.cpp
 *
 *  Created on: Aug 25, 2013
 *      Author: moritz
 */
#include "App.h"
#include "AppState.h"
#include "Text.h"

AppState::AppState(): caption("", App::white){
	timer = 0;
}

AppState::~AppState() {
	// TODO Auto-generated destructor stub
}

void AppState::draw(){
	
}
void AppState::next(int branch){
	App::get()->nextState(branch);
}
void AppState::pump(const SDL_Event &event){

}

