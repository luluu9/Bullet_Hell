#pragma once

// SETTINGS
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int MAX_FPS = 144;
const int MAX_CHARS = 120;
const bool FULLSCREEN = false;
static const char* TITLE = "Wojna Wydzialow";
const bool DRAW_COLLISION_BOX = false;


// SPECIFIC VALUES
const double PI = 3.14159265;


// PATHS
const unsigned short MAX_PATH_LENGTH = 255;
static char* SPARK_TXT_PATH = "./assets/spark.bmp";
static char* ACID_TXT_PATH = "./assets/acid.bmp";
static char* CHEMICZNY_TXT_PATH = "./assets/chemiczny.bmp";
static char* BACKGROUND_TXT_PATH = "./assets/sky.bmp";
static char* PLAYER_TXT_PATH = "./assets/etispinner_small.bmp";
static char* CHARSET_TXT_PATH = "./assets/charset8x8.bmp";
static char* ROBOT_TXT_PATH = "./assets/robot_small.bmp";
static char* AIR_TXT_PATH = "./assets/air.bmp";
static char* EMP_TXT_PATH = "./assets/emp_granade/emp_granade.bmp";
static char* EMPWave_TXT_PATH = "./assets/emp_granade/emp_wave_large.bmp";
static char* WILIS_TXT_PATH = "./assets/wilis.bmp";
static char* HAMMER_TXT_PATH = "./assets/hammer_small.bmp";
static char* WRECKING_BALL_TXT_PATH = "./assets/ball.bmp";
static char* WRECKING_BALL_SHADOW_TXT_PATH = "./assets/ball_shadow.bmp";
static char* GAS_TXT_PATH = "./assets/gas.bmp";


// ANIMATIONS
static char* EMP_GRANADE_DIR_PATH = "./assets/emp_granade";
static int EMP_GRANADE_FRAMES = 9;


// CHARACTERS
static int PLAYER_HP = 100;
static int CHEMICZNY_HP = 400;
static int AIR_HP = 500;
static int WILIS_HP = 700;

// CHEMICZNY
static int CHEMICZNY_SPEED = 350;
static int CHEMICZNY_SHOOTING_THRESHOLD = 700; // px
static int CHEMICZNY_SHOOTING_DELAY = 1500; // in msec
static int CHEMICZNY_SHOOTING_SIDES = 6; // in msec
static int CHEMICZNY_GAS_DELAY = 8000; // in msec
static int CHEMICZNY_GAS_SPEED = 10000; // how fast gas expands to fill GAS_WIDTH 
static int CHEMICZNY_GAS_HEIGHT = 600;
static int CHEMICZNY_GAS_WIDTH = SCREEN_WIDTH * 4;

// AIR
static int AIR_SPEED = 300;
static int AIR_SHOOTING_DELAY = 2500;
static int AIR_ROBOTS_NUMBER = 5;
static int AIR_ROBOTS_RADIUS = 250;
static int AIR_SHOOTING_THRESHOLD = 1000;

// WILIS
static int WILIS_HAMMER_SHOOTING_THRESHOLD = 600; // px
static int WILIS_HAMMER_SHOOTING_DELAY = 2000; // in msec
static int WILIS_BALL_SHOOTING_DELAY = 3000; 
static int WILIS_SPEED = 200; // px per second