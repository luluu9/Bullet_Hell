#include <stdio.h>
#include <cstdint>
#include <string.h>
#include "scores.h"
#include "settings.h"


int* Scores::getScores(int startIndex, int number) {
	if (!scoresLoaded) loadScores();
	return scores;
}

int Scores::getScoresNumber() {
	return currScore;
}

void Scores::addScore(char* nickname, int score) {
	if (!scoresLoaded) loadScores();
	for (int i = 0; i < currScore; i++) {
		if (score > scores[i]) {
			scores[i + 1] = scores[i];
			strcpy(nicknames[i+1], nicknames[i]);
			sprintf_s(scoreStrings[i+1], "%d", scores[i]);
			scores[i] = score;
			strcpy(nicknames[i], nickname);
			sprintf_s(scoreStrings[i], "%d", score);
			currScore++;
			saveScores();
			return;
		}
	}
	// if not returned, it means that score is lowest (on the end of scores);
	scores[currScore] = score;
	strcpy(nicknames[currScore], nickname);
	sprintf_s(scoreStrings[currScore], "%d", score);
	currScore++;
	saveScores();
}

void Scores::loadScores() {
	FILE* scoresFile = fopen(SCORES_PATH, "r");
	if (scoresFile == NULL) return;

	char nickname[READ_BUFFER];
	int score;
	while (fscanf(scoresFile, "%s %d", nickname, &score) != EOF) {
		scores[currScore] = score;
		strcpy(nicknames[currScore], nickname);
		sprintf_s(scoreStrings[currScore], "%d", score);
		currScore++;
	}

	fclose(scoresFile);
	scoresLoaded = true;
}

void Scores::saveScores() {
	FILE* scoresFile = fopen(SCORES_PATH, "w");
	if (scoresFile == NULL) return;

	for (int i = 0; i < currScore; i++) {
		fprintf(scoresFile, "%s %i\n", nicknames[i], scores[i]);
	}

	fclose(scoresFile);
}
