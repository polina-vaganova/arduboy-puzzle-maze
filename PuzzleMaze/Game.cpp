#include "Game.h"
#include "levels.h"
#include "graphics.h"
// #include <ArduboyTones.h>  // Звук

// extern ArduboyTones tones; // Звук

Game::Game() {
  gameState = MENU;
  gameStateNext = MENU;
  currentLevel = 1;
  camera = new Camera{0, 0, 0, 0};

  selectingSkin = false;
  selectedSkin = 0;

  loadLevel(1);
}

Game::~Game() {
  delete camera;
}

void Game::updateCamera() {
  if (gameState == PLAY) {

    int uiWidth = 58;
    int gameAreaWidth = 128 - uiWidth;

    // Центрируем игрока в игровой области (не на всем экране)
    camera->targetX = playerX * CELL_SIZE * ZOOM_IN - gameAreaWidth / 2;
    camera->targetY = playerY * CELL_SIZE * ZOOM_IN - 32; // По вертикали центрируем

    // Плавное движение камеры
    camera->x += (camera->targetX - camera->x) * 0.3f;
    camera->y += (camera->targetY - camera->y) * 0.3f;

    // Ограничения камеры с учетом UI
    float maxX = MAZE_WIDTH * CELL_SIZE * ZOOM_IN - gameAreaWidth;
    float maxY = MAZE_HEIGHT * CELL_SIZE * ZOOM_IN - 64;

    // Не даем камере выходить за левую границу и заходить в UI область
    camera->x = constrain(camera->x, 0, maxX);
    camera->y = constrain(camera->y, 0, maxY);

    // Дополнительная проверка: камера не должна показывать UI область
    if (camera->x + gameAreaWidth > MAZE_WIDTH * CELL_SIZE * ZOOM_IN) {
      camera->x = MAZE_WIDTH * CELL_SIZE * ZOOM_IN - gameAreaWidth;
    }
  } else {
    // В других режимах камера в начале
    camera->x = 0;
    camera->y = 0;
  }
}

void Game::update(Arduboy2 &arduboy) {
  updateCamera();

  if (selectingSkin) {
    // Режим выбора скина
    handleInputSkinSelection(arduboy);
  } else {
    // Обычная логика игры
    switch (gameState) {
      case MENU:
        handleInputMenu(arduboy);  // ← ВАЖНО: обработка меню когда gameState = MENU
        break;
      case TUTORIAL:
        handleInputTutorial(arduboy);
        break;
      case MEMORIZE:
        if (millis() - startTime > MEMORIZE_TIME) {
          gameState = PLAY;
          startTime = millis();
        }
        break;
      case PLAY:
        handleInput(arduboy);
        updateGame();
        if (arduboy.justPressed(B_BUTTON)) {
          gameState = PAUSE;
          pauseOptionIndex = 0;
        }
        break;
      case PAUSE:
        handleInputPause(arduboy);
        break;
      case WIN:
        if (arduboy.justPressed(A_BUTTON)) nextLevel();
        break;
      case GAME_OVER:
        if (arduboy.justPressed(A_BUTTON)) gameState = MENU;
        break;
      case VICTORY:
        handleInputVictory(arduboy);
        break;
    }
  }
}

void Game::startGame() {
  loadLevel(currentLevel);
  gameState = MEMORIZE;
  startTime = millis();
  if (currentLevel == 1) {
        gameStartTime = millis();
    }
  playerX = 1;
  playerY = 1;
  hasKey = false;
}


void Game::nextLevel() {
  if (currentLevel < 5) {
        currentLevel++;
        loadLevel(currentLevel);
        startGame();
    } else {
        gameState = VICTORY;
        victoryOptionIndex = 0;
        unsigned long totalTime = (millis() - gameStartTime) / 1000; // время в секундах
        victoryTime = totalTime;
    }
}

void Game::handleInput(Arduboy2 &arduboy) {
  int newX = playerX;
  int newY = playerY;

  if (arduboy.justPressed(UP_BUTTON)) newY--;
  if (arduboy.justPressed(DOWN_BUTTON)) newY++;
  if (arduboy.justPressed(LEFT_BUTTON)) newX--;
  if (arduboy.justPressed(RIGHT_BUTTON)) newX++;

  if (checkCollision(newX, newY)) {
    playerX = newX;
    playerY = newY;
  }

  if (arduboy.justPressed(A_BUTTON)) {
    if (maze[playerX][playerY] == 3) {
      hasKey = true;
      maze[playerX][playerY] = 0;
    }
    checkNeighborCell(playerX-1, playerY);
    checkNeighborCell(playerX+1, playerY);
    checkNeighborCell(playerX, playerY-1);
    checkNeighborCell(playerX, playerY+1);
  }
}


bool Game::checkCollision(int x, int y) {
  return (maze[x][y] != 1 && maze[x][y] != 2);
}

void Game::checkNeighborCell(int x, int y) {
  if (maze[x][y] == 2 && hasKey) {
    maze[x][y] = 0;
    hasKey = false;
  }
}

void Game::updateGame() {
  if (millis() - startTime > GAME_TIME) {
    gameState = GAME_OVER;
  }

  if (maze[playerX][playerY] == 4) {
    gameState = WIN;
  }
}

void Game::draw(Arduboy2 &arduboy) {
  if (selectingSkin) {
    drawSkinSelection(arduboy, selectedSkin);
  } else {
    switch (gameState) {
      case MENU:
        drawMenu(arduboy, menuIndex, soundOn);
        break;
      case MEMORIZE:
        drawMazeForMemorize(arduboy);
        drawUI(arduboy, startTime, currentLevel, hasKey, gameState);
        break;
      case PLAY:
        drawMaze(arduboy, false, playerX, playerY, ZOOM_IN, *camera);  // ← *camera
        drawPlayer(arduboy, playerX, playerY, ZOOM_IN, *camera, selectedSkin);      // ← *camera
        drawUI(arduboy, startTime, currentLevel, hasKey, gameState);
        break;
      case WIN:
        drawWinScreen(arduboy, currentLevel);
        break;
      case GAME_OVER:
        drawGameOver(arduboy);
        break;
      case TUTORIAL: {
        arduboy.setCursor(2, 10);
        int tutorialOptionsCurrent = (tutorialPage < tutorialPages - 1) ? 2 : 1;
        switch(tutorialPage) {
            case 0:
                arduboy.print("Use arrows to move.");
                arduboy.setCursor(2, 20);
                arduboy.print("A = action;");
                arduboy.setCursor(2, 30);
                arduboy.print("B = pause/menu");
                break;
            case 1:
                arduboy.print("Collect keys");
                arduboy.setCursor(2, 20);
                arduboy.print("to open doors.");
                arduboy.setCursor(2, 30);
                break;
            case 2:
                arduboy.print("You have only 3s");
                arduboy.setCursor(2, 20);
                arduboy.print("to remember all maze");
                break;
            case 3:
                arduboy.print("Reach the exit");
                arduboy.setCursor(2, 20);
                arduboy.print("to win.");
                arduboy.setCursor(2, 30);
                arduboy.print("Timer counts down!");
                break;
        }

        for(int i = 0; i < tutorialOptionsCurrent; i++) {
            int y = 45 + i * 8;
            arduboy.setCursor(0, y);
            arduboy.print(i == tutorialOptionIndex ? ">" : " ");
            arduboy.setCursor(8, y);
            if(tutorialPage < tutorialPages - 1) {
                arduboy.print(i == 0 ? "Next" : "Exit");
            } else {
                arduboy.print("Exit");
            }
      }
        break;
    }
      case VICTORY:
        drawVictoryScreen(arduboy);
        break;
      case PAUSE:
        arduboy.setCursor(25, 20);
        arduboy.print("PAUSE");
        const int pauseOptions = 2;
        static const char* options[pauseOptions] = {"Continue", "Menu"};
        for (int i = 0; i < pauseOptions; i++) {
            int y = 35 + i * 10;
            arduboy.setCursor(20, y);
            arduboy.print(i == pauseOptionIndex ? ">" : " ");
            arduboy.print(options[i]);
        }
        break;
    }
  }
}

void Game::handleInputMenu(Arduboy2 &arduboy) {
  if (arduboy.justPressed(UP_BUTTON)) {
    menuIndex--;
    if(menuIndex < 0) menuIndex = menuItems - 1;
    playSound(1000, 50);
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    menuIndex++;
    if(menuIndex >= menuItems) menuIndex = 0;
    playSound(1000, 50);
  }
  if (arduboy.justPressed(A_BUTTON)) {
    playSound(1500, 70);
    switch(menuIndex) {
      case 0: // START - переходим к выбору скина
        selectingSkin = true;
        selectedSkin = 0;
        break;
      case 1: // TUTORIAL - сразу переходим к обучению
        tutorialPage = 0;
        tutorialOptionIndex = 0;
        gameState = TUTORIAL;
        break;
      case 2: // SOUND - переключаем звук
        soundOn = !soundOn;
        break;
    }
  }
}

void Game::handleInputTutorial(Arduboy2 &arduboy) {
  int tutorialOptionsCurrent = (tutorialPage < tutorialPages - 1) ? 2 : 1;

  if (arduboy.justPressed(UP_BUTTON)) {
      tutorialOptionIndex--;
      if(tutorialOptionIndex < 0) tutorialOptionIndex = tutorialOptionsCurrent - 1;
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
      tutorialOptionIndex++;
      if(tutorialOptionIndex >= tutorialOptionsCurrent) tutorialOptionIndex = 0;
  }
  if (arduboy.justPressed(A_BUTTON)) {
      if (tutorialPage < tutorialPages - 1) {
          if(tutorialOptionIndex == 0) {
            tutorialPage++;
          } else {
            gameState = MENU;
            tutorialPage = 0;
            tutorialOptionIndex = 0;
          }
      } else {
          gameState = MENU;
          tutorialPage = 0;
          tutorialOptionIndex = 0;

      }
  }
    if (arduboy.justPressed(B_BUTTON)) {
    gameState = MENU;  // ← Возвращаемся в меню
    menuIndex = 0;     // ← Сбрасываем выбор в меню
  }
}

void Game::handleInputVictory(Arduboy2 &arduboy) {
    if (arduboy.justPressed(A_BUTTON)) {
        gameState = MENU;
        currentLevel = 1;
        menuIndex = 0;
    }
}

void Game::drawVictoryScreen(Arduboy2 &arduboy) {
    arduboy.setCursor(20, 20);
    arduboy.print("YOU WIN!");
    arduboy.setCursor(10, 30);
    arduboy.print("Time: ");
    arduboy.print(victoryTime);
    arduboy.print("s");
    arduboy.setCursor(20, 45);
    arduboy.print("> MENU");
}

void Game::handleInputPause(Arduboy2 &arduboy) {
    if (arduboy.justPressed(UP_BUTTON)) {
        pauseOptionIndex--;
        if (pauseOptionIndex < 0) pauseOptionIndex = pauseOptions - 1;
    }
    if (arduboy.justPressed(DOWN_BUTTON)) {
        pauseOptionIndex++;
        if (pauseOptionIndex >= pauseOptions) pauseOptionIndex = 0;
    }
    if (arduboy.justPressed(A_BUTTON)) {
        if (pauseOptionIndex == 0) {
            // Продолжить игру
            gameState = PLAY;
        } else if (pauseOptionIndex == 1) {
            // Вернуться в главное меню
            gameState = MENU;
            currentLevel = 1;
            menuIndex = 0;
        }
    }
}

void Game::handleInputSkinSelection(Arduboy2 &arduboy) {
  if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(UP_BUTTON)) {
    selectedSkin--;
    if (selectedSkin < 0) selectedSkin = NUM_SKINS - 1;
    if (soundOn) playSound(800, 50);
  }
  if (arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
    selectedSkin++;
    if (selectedSkin >= NUM_SKINS) selectedSkin = 0;
    if (soundOn) playSound(800, 50);
  }
  if (arduboy.justPressed(A_BUTTON)) {
    // Подтверждаем выбор и начинаем игру
    selectingSkin = false;
    startGame();
    if (soundOn) playSound(1200, 100);
  }
  if (arduboy.justPressed(B_BUTTON)) {
    // Отмена - возврат в меню
    selectingSkin = false;
    if (soundOn) playSound(600, 50);
  }
}

void Game::playSound(int freq, int dur) {
    // if (soundOn) tones.tone(freq, dur); Звук
}
