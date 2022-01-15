#include <stdio.h>
#include <string.h>
#include "settings.h"


char* COMMANDS[] = { "Position", "END" };
char* CHARACTERS[] = { "PLAYER", "CHEMICZNY", "AIR", "WILIS" };

const int READ_BUFFER = 128;

// check if target string is in arr
int in(char** arr, int arr_len, char* target) {
	for (int i = 0; i < arr_len; i++) {
		if (strncmp(arr[i], target, strlen(target)) == 0) {
			return 1;
		}
	}
	return 0;
}

void loadSettings() { // int level?
	FILE* settings = fopen(SETTINGS_PATH, "r");
	if (settings == NULL) return;
	char text[READ_BUFFER];
	int numOfCharacters = sizeof(CHARACTERS) / sizeof(char*);
	while (fscanf(settings, "%s", text) != EOF) {
		if (in(CHARACTERS, numOfCharacters, text)) {
			printf_s("%s\n", text);
			readCommand(settings);
		}
	}
	printf_s("Pozycja: %i, %i\n", PLAYER_START_POS_X, PLAYER_START_POS_Y);
	fclose(settings);
}

void readCommand(FILE* settings) {
	char text[READ_BUFFER];
	int numOfCommands= sizeof(COMMANDS) / sizeof(char*);
	while (fscanf(settings, "%s", text) != EOF) {
		if (in(COMMANDS, numOfCommands, text)) {
			if (strcmp(text, "Position ")) {
				fscanf(settings, "%i %i", &PLAYER_START_POS_X, &PLAYER_START_POS_Y);
				
			}
			else if (strcmp(text, "END"))
				break;
		}
	}
}