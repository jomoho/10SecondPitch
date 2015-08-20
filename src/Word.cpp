/*
 * Word.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#include "Word.h"


Word::Word(const string &word, int value) {
	this->word = string(word);
	this->value = value;
}

Word::~Word() {
	// TODO Auto-generated destructor stub
}

