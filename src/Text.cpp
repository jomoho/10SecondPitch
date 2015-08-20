/*
 * Text.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#include "Text.h"
#include "App.h"

Text::Text(const string &text, SDL_Color color, int font_id,
		TextRenderType type) {
	this->font_id = font_id;
	this->type = type;
	this->color = color;

	this->text = string(text);

	tex = 0;
	src_rect.x = src_rect.y = src_rect.w = src_rect.h = 0;
	updateTexture();
}
Text::Text() {
	Text("", App::white);
}

Text::~Text() {
	SDL_DestroyTexture(tex);
}

int Text::getFontId() const {
	return font_id;
}

void Text::setFontId(int fontId) {
	font_id = fontId;

	updateTexture();
}

const string& Text::getText() const {
	return text;
}

void Text::setText(const string& text) {
	this->text = string(text);

	updateTexture();
}

TextRenderType Text::getType() const {
	return type;
}

void Text::setType(TextRenderType type) {
	this->type = type;

	updateTexture();
}

void Text::drawLeft(int x, int y, double scale, double angle) {
	if(text == ""){
		return ;
	}
	SDL_Rect dest_rect = { x, y,(int) (src_rect.w * scale),(int) (src_rect.h * scale) };
	SDL_RenderCopyEx(App::get()->ren->renderer, tex, &src_rect, &dest_rect,
			angle, NULL, SDL_FLIP_NONE);
}

void Text::drawRight(int x, int y, double scale) {
	if(text == ""){
		return ;
	}
	SDL_Rect dest_rect = {(int) (x - src_rect.w * scale),(int) (y - src_rect.h * scale),
			(int) (src_rect.w * scale), (int) (src_rect.h * scale)};
	SDL_RenderCopyEx(App::get()->ren->renderer, tex, &src_rect, &dest_rect, 0,
	NULL, SDL_FLIP_NONE);
}

void Text::drawCenter(int x, int y, double scale) {
	if(text == ""){
		return ;
	}
	SDL_Rect dest_rect ={
		(int) (x - src_rect.w * scale * 0.5),
		(int) (y - src_rect.h * scale * 0.5),
		(int) (src_rect.w * scale), (int) (src_rect.h * scale)};
	SDL_RenderCopyEx(App::get()->ren->renderer, tex, &src_rect, &dest_rect, 0,
	NULL, SDL_FLIP_NONE);
}

void Text::updateTexture() {
	if(text == ""){
		FILE_LOG(logDEBUG3)<<"updateTexture empty string" << this->text;
		return ;

	}
	if (tex != 0) {
		SDL_DestroyTexture(tex);
	}
	//TODO: color
	SDL_Color color = { 255, 255, 255 };
	TTF_Font * font = App::get()->fonts[font_id];
	SDL_Surface * srf = 0;

	if (type == TXT_BLEND) {
		srf = TTF_RenderText_Blended(font, text.c_str(), color);
		if(srf == 0){
			FILE_LOG(logERROR) << "TTF_RenderText_Blended" << TTF_GetError();
		}
	} else {
		srf = TTF_RenderText_Solid(font, text.c_str(), color);
		
		if(srf == 0){
			FILE_LOG(logERROR) << "TTF_RenderText_Solid" << TTF_GetError();
		}

	}
	int w, h;
	TTF_SizeText(font, text.c_str(), &w, &h);
	
	src_rect.x = src_rect.y = 0;
	src_rect.w = w;
	src_rect.h = h;

	tex = SDL_CreateTextureFromSurface(App::get()->ren->renderer, srf);

	if(tex == 0){
		FILE_LOG(logERROR) << "SDL_CreateTextureFromSurface" << SDL_GetError();
	}

	SDL_FreeSurface(srf);
}
