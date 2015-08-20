/*
 * FeedbackState.h
 *
 *  Created on: Aug 25, 2013
 *      Author: moritz
 */

#ifndef FEEDBACKSTATE_H_
#define FEEDBACKSTATE_H_

#include "AppState.h"
#include "Text.h"
#include "Judge.h"

class FeedbackState :
	public AppState
{
public:
	FeedbackState(const string &text);
	virtual ~FeedbackState(void);
	virtual AppState * getNext(int branch);
	virtual void draw();
	virtual void pump(const SDL_Event &event);
	
	Text answer, suggestion;
	//string text;
	//vector<Text> screen_text;
	ScoreResult result;
};

#endif /* FEEDBACKSTATE_H_ */

