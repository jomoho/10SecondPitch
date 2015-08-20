/*
 * Word.h
 *
 *  Created on: Aug 24, 2013
 *      Author: moritz
 */

#ifndef WORD_H_
#define WORD_H_
#include <string>

using namespace std;
class Word {
public:
	Word(const string &word, int value = 3);
	Word(const Word &w):word(w.word), value(w.value){};
	virtual ~Word();
	int value;
	string word;
};

#endif /* WORD_H_ */
