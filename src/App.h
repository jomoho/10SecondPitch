/*
 * App.h
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#ifndef APP_H_
#define APP_H_

#include <vector>
#include <iostream>
#include "RendererSDL.h"
#include "Text.h"
#include "AppState.h"
#include "BodyPart.h"
#include "Judge.h"

#define IMG_BG 0
#define IMG_BODY 1
#define IMG_HANDS 2
#define IMG_FACE_THIN 3
#define IMG_FACE_MED 4
#define IMG_FACE_FAT 5
#define IMG_EYES 6
#define IMG_EYES_BLINK 7
#define IMG_MOUTH_TALK 8


using namespace std;

class App {
	
private:
	void loadFonts();
	void freeFonts();

	SDL_Texture * loadTexture(const string &filename);

	void loadTextures();
	void freeTextures();

	void parseBodyParts();
	void loadBodyPartTextures();

	void switchNextState();
	void drawBg();

	vector<string> bp_textures;

	int next_state;
public:
	App();
	virtual ~App();
	static App *instance ;
	static App *get();
	static void destroyInstance();

	void run();
	void start();

	void nextState(int branch = 0);
	AppState *state;
	RendererSDL *ren;
	bool done, debug;
	int tries, level;

	Judge judge;

	vector<BodyPart> bodyparts;
	vector<Uint32> body_ids, hair_ids, extra_ids;
	vector<TTF_Font *> fonts;
	vector<SDL_Texture *> textures;

	static SDL_Color black, white;
};

#endif /* APP_H_ */
