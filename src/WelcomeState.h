/*
 * WelcomeState.h
 *
 *  Created on: Aug 25, 2013
 *      Author: moritz
 */

#ifndef WELCOMESTATE_H_
#define WELCOMESTATE_H_

#include "AppState.h"

class WelcomeState :
	public AppState
{
public:
	WelcomeState(int level = 0);
	~WelcomeState(void);
	virtual AppState * getNext(int branch);
	virtual void draw();
	virtual void pump(const SDL_Event &event);
};

#endif /* WELCOMESTATE_H_ */


