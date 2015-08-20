#include <math.h>
#include <algorithm>

#include "Judge.h"
#include <time.h>
#include "App.h"

Judge::Judge(void)
{
	has_extra = false;

	dest_rect.x = 0;
	dest_rect.y = 0;
	dest_rect.w = 128;
	dest_rect.h = 128;
	
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.w = 128;
	src_rect.h = 128;
	srand(480);
}


Judge::~Judge(void)
{
}
void Judge::randomize(){
	face = (rand() % 3) + IMG_FACE_THIN;
	body = rand() % (int) App::get()->body_ids.size();
	hair = rand() % (int)  App::get()->hair_ids.size();
	extra = rand() % (int)  App::get()->extra_ids.size();

	body = App::get()->body_ids[body];
	hair = App::get()->hair_ids[hair];
	extra = App::get()->extra_ids[extra];

	has_extra = false;
	if(rand()%33 < 12){
		has_extra = true;
	}
}
void Judge::draw(int x, int y){
	App * app = App::get();
	SDL_Renderer * ren = App::get()->ren->renderer;

	dest_rect.x = x;
	dest_rect.y = y-128;


	double head_angle = sin((double)app->ren->frames/(head_nod/20))*8.0;
	SDL_Point pivot;
	pivot.x = 64;
	pivot.y = 96;
		/**
	#define IMG_BG 0
	#define IMG_BODY 1
	#define IMG_HANDS 2
	#define IMG_FACE_THIN 3
	#define IMG_FACE_MED 4
	#define IMG_FACE_FAT 5
	#define IMG_EYES 6
	#define IMG_EYES_BLINK 7
	#define IMG_MOUTH_TALK 8
	*/

	SDL_RenderCopyEx(ren, app->textures[IMG_BODY], &src_rect, &dest_rect, 0,
	NULL, SDL_FLIP_NONE);
	
	SDL_RenderCopyEx(ren, app->textures[app->bodyparts[body].tex_id], &src_rect, &dest_rect, 0,
	NULL, SDL_FLIP_NONE);

	SDL_RenderCopyEx(ren, app->textures[IMG_HANDS], &src_rect, &dest_rect, 0,
	NULL, SDL_FLIP_NONE);

	//face 
	dest_rect.y -= 64;

	SDL_RenderCopyEx(ren, app->textures[face], &src_rect, &dest_rect, head_angle,
	&pivot, SDL_FLIP_NONE);
	if(blink > 3){
		SDL_RenderCopyEx(ren, app->textures[IMG_EYES], &src_rect, &dest_rect, head_angle,
		&pivot, SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(ren, app->textures[IMG_EYES_BLINK], &src_rect, &dest_rect, head_angle,
		&pivot, SDL_FLIP_NONE);
	}


	SDL_RenderCopyEx(ren, app->textures[app->bodyparts[hair].tex_id], &src_rect, &dest_rect, head_angle,
	&pivot, SDL_FLIP_NONE);

	if(has_extra){
		SDL_RenderCopyEx(ren, app->textures[app->bodyparts[extra].tex_id], &src_rect, &dest_rect, head_angle,
		&pivot, SDL_FLIP_NONE);
	}

	if(talk > 0 && talk % 32 > 16){
		SDL_RenderCopyEx(ren, app->textures[IMG_MOUTH_TALK], &src_rect, &dest_rect, head_angle,
		&pivot, SDL_FLIP_NONE);
	}
	talk -= 1;
	blink -= 1;

	if(blink < 0){
		
		int r =  1 + rand() % 333;
		blink = 12 + (int)(app->ren->frames % r);
	}

}

int checkWord(const string &str, const Word &word ){
	return (str.find(word.word) != string::npos)? word.value : 0;
}

int checkBPNouns(const string &str, const BodyPart & part ){
	int s = 0;
	for(vector<Word>::size_type i =0; i < part.adjectives.size(); ++i){
		s = checkWord(str, part.nouns[i]);
		if(s > 0){
			return s;
		}
	}
	return s;
}



int checkBPDoubles(const string &str1, const string &str2, const BodyPart & part ){
	int s = 0;
	for(vector<Word>::size_type i =0; i < part.adjectives.size(); ++i){
		s = checkWord(str1, part.adjectives[i]);
		if(s > 0){
			s = s * checkWord(str2, part.nouns[i]);
			if(s > 0){
				return s;
			}
		}
	}
	return s;
}

vector<string> split(const string & str){
	vector<string> result;
	stringstream  data(str);

    string line;
    while(getline(data,line,' '))
    {
		if(line != "")
			result.push_back(line);
    }
	return result;
}

ScoreResult calcScoreForBP(const BodyPart & part, const vector<string> &text){
	ScoreResult result;

	int sub_score = 0, max_score = 0;
	for(vector<string>::size_type i=0; i< text.size(); ++i){
		sub_score = checkBPNouns(text[i], part);
		
		result.score += sub_score;

		if(sub_score > max_score){
			result.answer = text[i];
			max_score = sub_score;
		}

		if(i < text.size()-1){
			sub_score = checkBPDoubles(text[i], text[i+1],part);
			result.score += sub_score;

			if(sub_score > max_score){
				result.answer = text[i] + " " + text[i+1];
				max_score = sub_score;
			}
		}
	}
	return result;
}

ScoreResult Judge::calcScore(const string &txt){
	
	ScoreResult ret;

	App * app = App::get();
	vector<BodyPart> bp = app->bodyparts;
	vector<string> words = split(txt);

	ret.words = words.size();
	
	vector<ScoreResult> results;
	results.push_back(calcScoreForBP(bp[body], words));
	results[0].type = body;
	ret.score += results[0].score;
	results.push_back(calcScoreForBP(bp[hair], words));
	results[1].type = hair;
	ret.score += results[1].score;

	if(has_extra){
		results.push_back(calcScoreForBP(bp[extra], words));
		results[2].type = extra;
		ret.score += results[2].score;
	}

	std::sort(results.begin(), results.end());
	Uint32 endtype = results[results.size()-1].type;
	srand(App::get()->ren->frames);
	if(ret.score == 0){
		int r = rand() % 3;
		ret.answer = "Nice try...";
		ret.suggestion = "What about some " + bp[body].suggest(r);
	}

	if(ret.score > 0){
		int r = rand() % 3;
		ret.answer = "I like the " + results[0].answer + ".";		
		ret.suggestion = "But I love " + bp[endtype].suggest() + "!!!";
	}

	if(ret.score > 25){
		int r = rand() % 3;
		ret.answer = "I love the " + results[0].answer + "!";		
		ret.suggestion = "Only some " + bp[endtype].suggest(r) + " could add to it.";
	}

	if(ret.score > 50){
		int r = rand() % 3;
		ret.answer = "Fuck YEAH!!! " + results[0].answer + "!!!!";		
		ret.suggestion = "And some extra " + bp[endtype].suggest(r) + " pls!!!!";
	}

	if(ret.score > 75){
		int r = rand() % 3;
		ret.answer = "Northing to Add!! I love it!!! " + results[0].answer + " and "+ results[1].answer + "!!!!";		
		ret.suggestion = "Take my Money!!!";
	}
	FILE_LOG(logDEBUG3) << ret.answer << ret.suggestion << ret.score<< " words: " << ret.words;

	return ret;
}