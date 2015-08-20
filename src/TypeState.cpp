#include "WelcomeState.h"
#include "TypeState.h"
#include "FeedbackState.h"
#include "App.h"

TypeState::TypeState(void)
{

	txt = new Text("", App::white);
	name = "Type";
	instruction = string("Type! Type! Type! Enter to skip forwards.");
	SDL_StartTextInput();
	
	App::get()->judge.talk = 0;
	App::get()->judge.head_nod = 500;
}


TypeState::~TypeState(void)
{
	SDL_StopTextInput();
}

AppState * TypeState::getNext(int branch){
	if(branch == 33){
		return NULL;
	}
	return new FeedbackState(txt->getText() );
}
void TypeState::pump(const SDL_Event &event)
{	
	string str = txt->getText();
    if(event.type == SDL_TEXTINPUT){
        txt->setText(str + event.text.text);
    }

    if(event.type == SDL_KEYDOWN){
		if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
			if (str.size () > 0)  {
				str.resize (str.size () - 1);
			}			
			
			txt->setText(str);
		}

		if(event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_RETURN2 || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
			SDL_StopTextInput();
			next();
		}
	}
}

void TypeState::draw(){
	SDL_Rect time_rect;

	time_rect.x = 20;
	time_rect.y = 20;
	time_rect.w = App::get()->ren->width-40;
	time_rect.h = 32;

	double percent_done = (double)AppState::timer/ 10000.0;
	SDL_SetRenderDrawBlendMode(App::get()->ren->renderer, SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawColor(App::get()->ren->renderer, 255, 255, 255, 64);
	SDL_RenderFillRect(App::get()->ren->renderer, &time_rect);

	time_rect.x += 6;
	time_rect.y += 6;
	time_rect.w = (time_rect.w-12)*percent_done;
	time_rect.h -= 12;

	SDL_SetRenderDrawColor(App::get()->ren->renderer, 255, 255, 255, 128);
	SDL_RenderFillRect(App::get()->ren->renderer, &time_rect);
	SDL_SetRenderDrawBlendMode(App::get()->ren->renderer, SDL_BLENDMODE_NONE);


	txt->drawRight(App::get()->ren->width-20, App::get()->ren->height-60, 2.0);
	
	txt->drawCenter(App::get()->ren->width/3, 3*App::get()->ren->height/8,1.5);
	txt->drawRight(2*App::get()->ren->width/3, 5*App::get()->ren->height/8,2.5);
	txt->drawCenter(20, App::get()->ren->height/3, 3.0);
	//THE 10 SECONDS RULE
	if(AppState::timer > 10000){
		next();
	}
}
