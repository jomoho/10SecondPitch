#include "WelcomeState.h"
#include "TypeState.h"
#include "FeedbackState.h"
#include "App.h"
#include <math.h>


WelcomeState::WelcomeState(int level) {

	name = "Welcome";
	instruction = "Press any key to Start.";

	caption.setText("Hey Dude... tell me about your game");

	if(level > App::get()->level){
		App::get()->judge.randomize();
		App::get()->level = level;
		App::get()->tries = 0;
	}
		
	App::get()->tries += 1;

	App::get()->judge.talk = 240;
	App::get()->judge.head_nod = 1000;
	//App::get()->judge.calcScore("difficult puzzle blocky pixelart complex levels face0011.png: chunky bacon unusual mechanics complex level ");
}
WelcomeState::~WelcomeState(void) {

}

AppState * WelcomeState::getNext(int branch){
	if(branch == 33){
		return NULL;
	}
	return new TypeState();
}
void WelcomeState::draw(){
	stringstream s ;
	s << "Level " << ( App::get()->level) << " Round " <<(App::get()->tries);
	Text txt = Text(s.str(), App::white, 1, TXT_BLEND);

	txt.drawCenter(App::get()->ren->width/2, 1.5*App::get()->ren->height/8  , 2.0);
	caption.drawCenter(App::get()->ren->width/2, 5.25*App::get()->ren->height/8  , 2.0  );
}
void WelcomeState::pump(const SDL_Event &event){
	if(event.type == SDL_KEYDOWN){
		next();
	}
}