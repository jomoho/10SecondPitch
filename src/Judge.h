/*
 * Judge.h
 *
 *  Created on: Aug 25, 2013
 *      Author: moritz
 */

#ifndef JUDGE_H_
#define JUDGE_H_

#include "Word.h"
#include "RendererSDL.h"
#include "BodyPart.h"
#include <vector>

using namespace std;

class ScoreResult{
public:
	ScoreResult(){
		type = 0;
		score = 0;
		words = 0;
	}
	int type;
	int score;
	int words;
	string answer, suggestion;
	bool operator< (const ScoreResult &r2){ return score > r2.score; };
};

class Judge
{
public:
	Judge(void);
	~Judge(void);
	SDL_Rect src_rect, dest_rect;

	Uint32 face, body, hair, extra;

	int talk, blink; //milliseconds to talk

	bool has_extra;
	
	void randomize();
	void draw(int x, int y);

	int head_nod;

	ScoreResult calcScore(const string &txt);
};


#endif /* JUDGE_H_ */
