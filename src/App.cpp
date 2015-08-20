/*
 * App.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iterator>
#include <typeinfo>

#include "App.h"
#include "Log.h"
#include "SDL_image.h"

#include "WelcomeState.h"

SDL_Color App::black = {0, 0, 0};
SDL_Color App::white = {255,255,255};

App::App() {
	FILELog::ReportingLevel() = logDEBUG3;
	FILE* log_fd = fopen( "logfile.txt", "w" );
	Output2FILE::Stream() = log_fd;

	ren = new RendererSDL("10 Second Indie Game Pitch", 640, 480, 60);
	done = false;
	debug = true;
	next_state = 0;
	level = 0;
	tries = 0;


	// load support for PNG images
	int flags = IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if((initted&flags) != flags) {
		FILE_LOG(logWARNING) << "Error initializing SDL_image" << IMG_GetError();
	}

	loadFonts();
	FILE_LOG(logDEBUG) << "Fonts Loaded!";
	loadTextures();
	FILE_LOG(logDEBUG) << "Textures Loaded!";
	
	parseBodyParts();
	FILE_LOG(logDEBUG) << "Body Parts Loaded!";
	FILE_LOG(logDEBUG) << "body_ids: " << this->body_ids.size()
					   << " hair_ids: " << this->hair_ids.size()
					   << " extra_ids: " << this->extra_ids.size();
	IMG_Quit();
}


App::~App() {
	freeTextures();
	
	freeFonts();
	SDL_StopTextInput();
	TTF_Quit();

	delete ren;
}

App * App::instance = 0;

App* App::get() {
	if(instance == 0){
		instance = new App();
	}
	return instance;
}

void App::destroyInstance() {
	if(instance != 0){
		delete instance;
	}
	instance = 0;
}

void App::loadFonts() {
	TTF_Init();
	TTF_Font *font;
	std::string fontfiles [] = {"fonts/DroidSansMono.ttf", "fonts/DroidSansMono.ttf"};
	int fontsizes [] = {12,16};
	for(int i= 0; i <2; i++){
		font = TTF_OpenFont(fontfiles[i].c_str(), fontsizes[i]);
		if(font == NULL){
			FILE_LOG(logERROR)  << "TTF_OpenFont Error: " << TTF_GetError();
		}else{
			FILE_LOG(logDEBUG)  << "Font Loaded: " << fontfiles[i];
			fonts.push_back(font);
		}
	}
}

void App::freeFonts() {
	for(vector<TTF_Font*>::size_type i = 0; i < fonts.size(); ++i) {
	    TTF_CloseFont(fonts[i]);
	}
	fonts.empty();
	TTF_Quit();
}

SDL_Texture * App::loadTexture(const string &filename){
	SDL_Surface *srf;
	srf = IMG_Load_RW(SDL_RWFromFile(filename.c_str(), "rb"), 1);
	if(!srf) {
		FILE_LOG(logERROR)  << "loadTexture Error: " << IMG_GetError();
	}
	SDL_Texture * tex;
	tex = SDL_CreateTextureFromSurface(ren->renderer, srf);

	SDL_FreeSurface(srf);
	return tex;
}

void App::loadTextures() {
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
	textures.push_back(loadTexture("gfx/bg.png"));
	textures.push_back(loadTexture("gfx/body0002.png"));
	textures.push_back(loadTexture("gfx/body0011.png"));
	textures.push_back(loadTexture("gfx/face0004.png"));
	textures.push_back(loadTexture("gfx/face0005.png"));
	textures.push_back(loadTexture("gfx/face0006.png"));
	textures.push_back(loadTexture("gfx/face0008.png"));
	textures.push_back(loadTexture("gfx/face0010.png"));
	textures.push_back(loadTexture("gfx/face0023.png"));

}

void App::loadBodyPartTextures() {
	for(vector<string>::size_type i = 0; i < bp_textures.size(); ++i){
		textures.push_back(loadTexture(bp_textures[i]));
	}
}

void App::freeTextures() {
	for(vector<SDL_Texture*>::size_type i = 0; i < textures.size(); ++i) {
	    SDL_DestroyTexture(textures[i]);
	}
	textures.empty();
}

SDL_Event event;

void App::start() {
	state = new WelcomeState(1);
}
void App::drawBg(){
		SDL_Rect src_rect, dest_rect;
		dest_rect.x = 0;
		dest_rect.y = 0;
		dest_rect.w = ren->width;
		dest_rect.h = ren->height;
	
		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.w = 64;
		src_rect.h = 48;

		SDL_RenderCopy(ren->renderer,textures[IMG_BG], &src_rect, &dest_rect);
}
void App::run() {

    while(SDL_PollEvent(&event))      // Check for events.
    {
		state->pump(event);

        if(event.type == SDL_QUIT || event.type == SDL_QUIT)
            done = true;
    }
	string str = state->instruction ;
	Text txt( str , App::white);
    ren->startFrame();
		drawBg();
		state->draw();
		judge.draw(ren->width/2 -64, (ren->height/8)* 5);

		
		txt.drawCenter(ren->width/2, ren->height-32);
    ren->endFrame();
	/**
	if(ren->frames % (60) == 0){
		judge.randomize();
	}
	**/

	state->timer += ren->tick_diff;
	if(next_state > 0){
		switchNextState();
	}
}
Uint32 getCategoryId(string category){
	if(category == "body" || category[0] == '1'){
		return 0;
	}
	if(category == "hair" || category[0]  == '2'){
		return 1;
	}
	if(category == "extra" || category[0]  == '3'){
		return 2;
	}

	return 77;
}

string join(const vector<Uint32>& vec, const char* delim)
{
    string res;
	char buf[32];
	for(vector<Uint32>::size_type i = 0; i< vec.size(); ++i){
		res = res + std::to_string(vec[i]) + delim;
	}
    return res;
}

void App::parseBodyParts(){	
	ifstream infile("gfx/bodyparts.txt");
	string line;

	string prefix = "gfx/";

	string adj, noun, name;
	Uint32 val, bodypart_id = 0, category_id = 0;

	while (getline(infile, line))
	{
		istringstream iss(line);

		string::size_type pos = line.find(':');
		if( pos != string::npos){			

			name = line.substr(0, pos);
			category_id = getCategoryId(line.substr(pos+1, string::npos));

			FILE_LOG(logDEBUG3) << name <<" <-- "<< category_id;
			
			bodypart_id = bodyparts.size();
			bodyparts.push_back(BodyPart());

			bodyparts[bodypart_id].tex_id = textures.size() + bp_textures.size();
			bp_textures.push_back(prefix+name);


			bodyparts[bodypart_id].category_id = category_id;
			FILE_LOG(logDEBUG3) << prefix+name << "tex: " << bodyparts[bodypart_id].tex_id << " cat: " << bodyparts[bodypart_id].category_id;

			switch(category_id){
				case 0:
					body_ids.push_back(bodypart_id);
					break;
				case 1:
					hair_ids.push_back(bodypart_id);
					break;
				case 2:
					extra_ids.push_back(bodypart_id);
					break;
			}
			FILE_LOG(logDEBUG3) << "b: " << join(body_ids, ", ") << " h: "<< join(hair_ids, ", ") << " e: "<< join(extra_ids, ", ");

		} else if(line != ""){
			if(iss >> adj >> noun >> val) {

				bodyparts[bodypart_id].adjectives.push_back(Word(adj, val));
				bodyparts[bodypart_id].nouns.push_back(Word(noun, val));
				
				FILE_LOG(logDEBUG3) << "adjective: " << adj << " noun: " << noun << " value: "<< val << " tostr: " << bodypart_id;//bodyparts[bodypart_id].to_string();
			} else{
				FILE_LOG(logWARNING) << "parseBodyParts error on line: " << line;
			}
		}
	}
	infile.close();
	loadBodyPartTextures();
}

void App::nextState(int branch){
	FILE_LOG(logDEBUG3)<< state->name << "->>nextState " << branch ;
	next_state = branch;
}

void App::switchNextState(){
	AppState * new_state = state->getNext(next_state);

	if(new_state == NULL){		
		FILE_LOG(logDEBUG3) << "exit strategy " << next_state;
		done = true;
	} else {
		delete state;
		state = new_state;
		FILE_LOG(logDEBUG3) << "STATE: "<<state->name;
	}
	
	next_state = -1;
}
