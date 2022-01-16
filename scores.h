#pragma once
#include "settings.h"


class Scores {
public:
	char nicknames[MAX_SCORES][MAX_CHARS];
	char scoreStrings[MAX_SCORES][MAX_CHARS];

	int* getScores();
	int getScoresNumber();
	void addScore(char* nickname, int score);
private:
	void loadScores();
	void saveScores();
	int scores[MAX_SCORES];
	
	int currScore = 0;
	bool scoresLoaded = false;
};

