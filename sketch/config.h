#line 1 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\config.h"
#ifndef CONFIG_H
#define CONFIG_H

const int CELL_SIZE = 7;
const int PLAYER_SIZE = 5;
const int MAZE_WIDTH = 16;
const int MAZE_HEIGHT = 12;

const unsigned long MEMORIZE_TIME = 3000;
const unsigned long GAME_TIME = 60000;

const float ZOOM_OUT = 0.8f;
const float ZOOM_IN = 1.0f;
const int VIEW_RADIUS = 2;

const int NUM_SKINS = 4;
const int SKIN_SIZE = 5;

struct Camera {
  float x, y;
  float targetX, targetY;
};

enum GameState { MENU, MEMORIZE, PLAY, WIN, GAME_OVER, TUTORIAL, VICTORY, PAUSE };

#endif