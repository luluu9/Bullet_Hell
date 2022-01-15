#pragma once

// SETTINGS
static const char* TITLE = "Wojna Wydzialow";
static const char* SETTINGS_PATH = "./settings.txt";
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int MAX_FPS = 144;
const int MAX_CHARS = 120;
const int DEST_CHAR_SIZE = 32;
const bool FULLSCREEN = false;
const bool DRAW_COLLISION_BOX = false;


// SPECIFIC VALUES
const double PI = 3.14159265;


// PATHS
const unsigned short MAX_PATH_LENGTH = 255;
extern char* SPARK_TXT_PATH;
extern char* ACID_TXT_PATH;
extern char* CHEMICZNY_TXT_PATH;
extern char* BACKGROUND_TXT_PATH;
extern char* PLAYER_TXT_PATH;
extern char* CHARSET_TXT_PATH;
extern char* ROBOT_TXT_PATH;
extern char* AIR_TXT_PATH;
extern char* EMP_TXT_PATH;
extern char* EMPWave_TXT_PATH;
extern char* WILIS_TXT_PATH;
extern char* HAMMER_TXT_PATH;
extern char* WRECKING_BALL_TXT_PATH;
extern char* WRECKING_BALL_SHADOW_TXT_PATH;
extern char* GAS_TXT_PATH;
extern char* BONUS_ENTITY_PATH;


// ANIMATIONS
extern char* EMP_GRANADE_DIR_PATH;
extern int EMP_GRANADE_FRAMES;


// LEVEL SETTINGS
extern bool LEVEL_CHEMICZNY;
extern bool LEVEL_AIR;
extern bool LEVEL_WILIS;


// CHARACTERS
extern int PLAYER_HP;
extern int CHEMICZNY_HP;
extern int AIR_HP;
extern int WILIS_HP;

// PLAYER
extern int ATTACK_SCORE;
extern int DMG_SCORE;
extern int PLAYER_START_POS_X;
extern int PLAYER_START_POS_Y;

// CHEMICZNY
extern int CHEMICZNY_SPEED;
extern int CHEMICZNY_SHOOTING_THRESHOLD;
extern int CHEMICZNY_SHOOTING_DELAY;
extern int CHEMICZNY_SHOOTING_SIDES;
extern int CHEMICZNY_GAS_DELAY;
extern int CHEMICZNY_GAS_SPEED;
extern int CHEMICZNY_GAS_HEIGHT;
extern int CHEMICZNY_GAS_WIDTH;
extern int CHEMICZNY_START_POS_X;
extern int CHEMICZNY_START_POS_Y;

// AIR
extern int AIR_SPEED;
extern int AIR_SHOOTING_DELAY;
extern int AIR_ROBOTS_NUMBER;
extern int AIR_ROBOTS_RADIUS;
extern int AIR_SHOOTING_THRESHOLD;
extern int AIR_START_POS_X;
extern int AIR_START_POS_Y;

// WILIS
extern int WILIS_HAMMER_SHOOTING_THRESHOLD;
extern int WILIS_HAMMER_SHOOTING_DELAY;
extern int WILIS_BALL_SHOOTING_DELAY;
extern int WILIS_SPEED;
extern int WILIS_START_POS_X;
extern int WILIS_START_POS_Y;


void loadSettings(int level);
void resetSettings();
void readCommand(FILE* settings, char* character);
void changeValue(FILE* settings, char* character, char* command);