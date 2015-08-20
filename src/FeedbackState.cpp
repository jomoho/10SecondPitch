#include "WelcomeState.h"
#include "TypeState.h"
#include "FeedbackState.h"
#include "App.h"
#include "Log.h"

FeedbackState::FeedbackState(const string &text): answer("", App::white, 1, TXT_BLEND), suggestion("", App::white, 1, TXT_BLEND){
	name = "Feedback";
	instruction = string("Press Enter to Continue.");
//	this->text = string(text);
	
	FILE_LOG(logDEBUG3) << "FeedbackState cstruct " << text;
	result = App::get()->judge.calcScore(text);
	/*
	screen_text.push_back(Text(text, App::white));
	screen_text.push_back(Text(result.answer, App::white));
	screen_text.push_back(Text(result.suggestion, App::white));
	*/
//	FILE_LOG(logDEBUG3) << "FeedbackState cstruct " << screen_text[1].getText();
	stringstream s;
	s << "Score: " << result.score;
	caption.setText(s.str());
	
	answer.setText(result.answer);
	suggestion.setText(result.suggestion);

	FILE_LOG(logDEBUG3) << "FeedbackState cstruct " << caption.getText();
	App::get()->judge.talk = 600;
	App::get()->judge.head_nod = 700 -2*result.score;
	
}


FeedbackState::~FeedbackState(void)
{
}

AppState * FeedbackState::getNext(int branch){
	if(branch == 999){
		return NULL;
	}
	int level = App::get()->level;
	if(result.score > 50){
		level += 1;
	}
	return new WelcomeState(level);
	
}
void FeedbackState::draw(){
	caption.drawCenter(App::get()->ren->width/2, 2.7*App::get()->ren->height/4, 3.0  );
	
	double scale = 0.95* App::get()->ren->width/answer.src_rect.w;
	answer.drawCenter(App::get()->ren->width/2, 1*App::get()->ren->height/8, scale );
	
	suggestion.drawCenter(App::get()->ren->width/2, 6.5*App::get()->ren->height/8, 1.0 );
	
	/*screen_text[0].drawCenter(App::get()->ren->width/2, App::get()->ren->height/4 , 2.0  );
	screen_text[1].drawCenter(App::get()->ren->width/2, 3*App::get()->ren->height/4 + 30, 2.0  );
	screen_text[2].drawCenter(App::get()->ren->width/2, 3*App::get()->ren->height/4 + 60, 2.0  );
	*/
		
}
void FeedbackState::pump(const SDL_Event &event){
	if(event.type == SDL_KEYDOWN){
		if(event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_RETURN2 ){
			next();
		}
	}
}