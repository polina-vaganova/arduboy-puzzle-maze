#ifndef GAME_H
#define GAME_H
#define SPEAKER_PIN 4

#include <Arduboy2.h>
#include "config.h"

class Game {
public:
  Game();
  ~Game();
  void update(Arduboy2 &arduboy);
  void draw(Arduboy2 &arduboy);
  void startGame();
  void nextLevel();

private:
  void handleInput(Arduboy2 &arduboy);
  void updateGame();
  void checkNeighborCell(int x, int y);
  bool checkCollision(int x, int y);
  void handleInputMenu(Arduboy2 &arduboy);
  void handleInputTutorial(Arduboy2 &arduboy);
  void playSound(int freq, int dur);
  void drawTutorialPage(Arduboy2 &arduboy, int page);
  void drawVictoryScreen(Arduboy2 &arduboy);
  void handleInputVictory(Arduboy2 &arduboy);
  void handleInputPause(Arduboy2 &arduboy);
  void updateCamera();
  void handleInputSkinSelection(Arduboy2 &arduboy);

  GameState gameState;
  GameState gameStateNext;
  Camera* camera;
  int playerX, playerY;
  bool hasKey;
  unsigned long startTime;
  int currentLevel;
  int menuIndex = 0;
  const int menuItems = 3;
  bool soundOn = true;
  int tutorialPage = 0;
  int tutorialPages = 4;
  int tutorialOptionIndex = 0;
  int tutorialOptions = 2;
  int victoryOptionIndex = 0;
  int pauseOptionIndex = 0;
  const int pauseOptions = 2;
  unsigned long gameStartTime;
  unsigned long victoryTime;
  float cameraX, cameraY;
  int selectedSkin = 0;
  bool selectingSkin = false;
};

#endif
