#include <stdio.h>
#include <string.h>
#include "settings.h"


// PATHS
char* SPARK_TXT_PATH = "./assets/spark.bmp";
char* ACID_TXT_PATH = "./assets/acid.bmp";
char* CHEMICZNY_TXT_PATH = "./assets/chemiczny.bmp";
char* BACKGROUND_TXT_PATH = "./assets/sky.bmp";
char* PLAYER_TXT_PATH = "./assets/etispinner_small.bmp";
char* CHARSET_TXT_PATH = "./assets/charset8x8.bmp";
char* ROBOT_TXT_PATH = "./assets/robot_small.bmp";
char* AIR_TXT_PATH = "./assets/air.bmp";
char* EMP_TXT_PATH = "./assets/emp_granade/emp_granade.bmp";
char* EMPWave_TXT_PATH = "./assets/emp_granade/emp_wave_large.bmp";
char* WILIS_TXT_PATH = "./assets/wilis.bmp";
char* HAMMER_TXT_PATH = "./assets/hammer_small.bmp";
char* WRECKING_BALL_TXT_PATH = "./assets/ball.bmp";
char* WRECKING_BALL_SHADOW_TXT_PATH = "./assets/ball_shadow.bmp";
char* GAS_TXT_PATH = "./assets/gas.bmp";
char* BONUS_ENTITY_PATH = "./assets/bonus.bmp";
char* SCOREBOARD_PATH = "./assets/scoreboard.bmp";


// ANIMATIONS
char* EMP_GRANADE_DIR_PATH = "./assets/emp_granade";
int EMP_GRANADE_FRAMES = 9;


// LEVEL SETTINGS
bool LEVEL_CHEMICZNY = 0;
bool LEVEL_AIR = 0;
bool LEVEL_WILIS = 0;


// CHARACTERS
int PLAYER_HP = 100;
int CHEMICZNY_HP = 400;
int AIR_HP = 500;
int WILIS_HP = 600;

// PLAYER
int ATTACK_SCORE = 1;
int DMG_SCORE = 100;
int PLAYER_START_POS_X;
int PLAYER_START_POS_Y;

// CHEMICZNY
int CHEMICZNY_SPEED = 350;
int CHEMICZNY_SHOOTING_THRESHOLD = 700; // px
int CHEMICZNY_SHOOTING_DELAY = 1500; // in msec
int CHEMICZNY_SHOOTING_SIDES = 6; // in msec
int CHEMICZNY_GAS_DELAY = 8000; // in msec
int CHEMICZNY_GAS_SPEED = 10000; // how fast gas expands to fill GAS_WIDTH 
int CHEMICZNY_GAS_HEIGHT = 600;
int CHEMICZNY_GAS_WIDTH = SCREEN_WIDTH * 4;
int CHEMICZNY_START_POS_X = 0;
int CHEMICZNY_START_POS_Y = 0;

// AIR
int AIR_SPEED = 300;
int AIR_SHOOTING_DELAY = 2500;
int AIR_ROBOTS_NUMBER = 5;
int AIR_ROBOTS_RADIUS = 250;
int AIR_SHOOTING_THRESHOLD = 1000;
int AIR_START_POS_X = 0;
int AIR_START_POS_Y = 0;

// WILIS
int WILIS_HAMMER_SHOOTING_THRESHOLD = 600; // px
int WILIS_HAMMER_SHOOTING_DELAY = 2000; // in msec
int WILIS_BALL_SHOOTING_DELAY = 3000;
int WILIS_SPEED = 200; // px per second
int WILIS_START_POS_X = 0;
int WILIS_START_POS_Y = 0;


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

void loadSettings(int level) { // int level?
	FILE* settings = fopen(SETTINGS_PATH, "r");
	if (settings == NULL) return;

	resetSettings();

	char text[READ_BUFFER];
	int numOfCharacters = sizeof(CHARACTERS) / sizeof(char*);
	int currentLevel;

	while (fscanf(settings, "%s", text) != EOF) {
		if (strcmp(text, "LEVEL") == 0)
			fscanf(settings, "%i", &currentLevel);
		if (level == currentLevel) {
			if (in(CHARACTERS, numOfCharacters, text)) {
				readCommand(settings, text);
			}
		}
	}
	fclose(settings);
}

void resetSettings() {
	LEVEL_CHEMICZNY = 0;
	LEVEL_AIR = 0;
	LEVEL_WILIS = 0;
}

void readCommand(FILE* settings, char* character) {
	char text[READ_BUFFER];
	int numOfCommands = sizeof(COMMANDS) / sizeof(char*);
	while (fscanf(settings, "%s", text) != EOF) {
		if (strcmp(text, "END") == 0) break;
		if (in(COMMANDS, numOfCommands, text)) {
			changeValue(settings, character, text);
		}
	}
}

void changeValue(FILE* settings, char* character, char* command) {
	if (strcmp(command, "Position") == 0) {
		if (strcmp(character, "PLAYER") == 0)
			fscanf(settings, "%i %i", &PLAYER_START_POS_X, &PLAYER_START_POS_Y);
		else if (strcmp(character, "CHEMICZNY") == 0) {
			LEVEL_CHEMICZNY = true;
			fscanf(settings, "%i %i", &CHEMICZNY_START_POS_X, &CHEMICZNY_START_POS_Y);
		}
		else if (strcmp(character, "AIR") == 0) {
			LEVEL_AIR = true;
			fscanf(settings, "%i %i", &AIR_START_POS_X, &AIR_START_POS_Y);
		}
		else if (strcmp(character, "WILIS") == 0) {
			LEVEL_WILIS = true;
			fscanf(settings, "%i %i", &WILIS_START_POS_X, &WILIS_START_POS_Y);
		}
	}

}