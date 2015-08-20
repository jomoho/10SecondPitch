/*
 * BodyPart.h
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#ifndef BODYPART_H_
#define BODYPART_H_

#include "Word.h"
#include "RendererSDL.h"
#include <vector>
using namespace std;

typedef vector<Word> vww;

class BodyPart {
public:
	BodyPart();
	virtual ~BodyPart();
	Uint32 tex_id, category_id;	
	vww adjectives;
	vww nouns;
	string suggest(vww::size_type r = 0){
		if(adjectives.size() > r && nouns.size() > r){
			return adjectives[r].word + " " + nouns[r].word;
		}
		return "";
	};

	string to_string(){
		stringstream s;
		for(vww::size_type i =0; i< MIN(nouns.size(), adjectives.size()); ++i){
			s << adjectives[i].word << " " << nouns[i].word << " " << nouns[i].value;
		}
		return s.str();
	}
	
	

};

#endif /* BODYPART_H_ */
