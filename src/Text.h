/*
 * Text.h
 *
 *  Created on: Aug 24, 2013
 *      Author: Moritz Laass
 */

#ifndef TEXT_H_
#define TEXT_H_
#ifdef _MSC_VER
	#include "SDL_ttf.h"
#else
	#include "SDL2/SDL_ttf.h"
#endif

#include <string>
#include "RendererSDL.h"

using namespace std;

typedef enum {
	TXT_BLIT, TXT_BLEND
} TextRenderType;

class Text {
public:
	Text();
	Text(const string &text, SDL_Color color, int font_id = 0,
			TextRenderType type = TXT_BLIT);
	virtual ~Text();

	void drawLeft(int x, int y, double scale = 1.0, double angle = 0);
	void drawRight(int x, int y, double scale = 1.0);
	void drawCenter(int x, int y, double scale = 1.0);

	int getFontId() const;
	void setFontId(int fontId);
	const string& getText() const;
	void setText(const string& text);
	TextRenderType getType() const;
	void setType(TextRenderType type);

	SDL_Rect src_rect;

private:
	void updateTexture();

	string text;
	int font_id;
	TextRenderType type;

	SDL_Color color;
	SDL_Texture *tex;
};

#endif /* TEXT_H_ */
