#line 1 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\graphics.h"
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduboy2.h>
#include "config.h"

void drawMazeForMemorize(Arduboy2 &arduboy);
void drawMazeCell(Arduboy2 &arduboy, int x, int y, int size, byte cellType);
void drawMaze(Arduboy2 &arduboy, bool showAll, int playerX, int playerY, float zoom = 1.0f, const Camera& camera = {0,0,0,0});
void drawPlayer(Arduboy2 &arduboy, int playerX, int playerY, float zoom, const Camera& camera, int skinIndex = 0);
void drawUI(Arduboy2 &arduboy, unsigned long startTime, int currentLevel, bool hasKey, GameState gameState);
void drawMenu(Arduboy2 &arduboy, int menuIndex, bool soundOn);
void drawWinScreen(Arduboy2 &arduboy, int currentLevel);
void drawGameOver(Arduboy2 &arduboy);
void drawSkinSelection(Arduboy2 &arduboy, int selectedSkin);

#endif