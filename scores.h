#pragma once
#include "settings.h"

const int READ_BUFFER = 128;
const int MAX_SCORES = 128;

class Scores {
public:
	//~Scores();

	char nicknames[MAX_SCORES][MAX_CHARS];
	char scoreStrings[MAX_SCORES][MAX_CHARS];

	int* getScores(int startIndex = 0, int number = 10);
	int getScoresNumber();
	void addScore(char* nickname, int score);
private:
	void loadScores();
	void saveScores();
	int scores[MAX_SCORES];
	
	int currScore = 0;
	bool scoresLoaded = false;
};

