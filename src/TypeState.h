/*
 * TypeState.h
 *
 *  Created on: Aug 25, 2013
 *      Author: moritz
 */

#ifndef TYPESTATE_H_
#define TYPESTATE_H_

#include "AppState.h"
#include "Text.h"
class TypeState :
	public AppState
{
	Text * txt;
public:
	TypeState(void);
	virtual ~TypeState(void);
	virtual AppState * getNext(int branch);
	virtual void draw();
	virtual  void pump(const SDL_Event &event);
	
	//Text screen_text[5];
};

#endif /* TYPESTATE_H_ */


