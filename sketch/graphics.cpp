#line 1 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\graphics.cpp"
#include "graphics.h"
#include "levels.h"

const byte playerSkins[4][5] PROGMEM = {
  {
    0b00000000,
    0b00001000, 
    0b00001100,
    0b00111000,
    0b00011000
  },
  {
    0b00000000,
    0b00100000,
    0b00011000,
    0b01111000,
    0b01110000
  },
  {
    0b00000000,
    0b10000000,
    0b10011000,
    0b01110000,
    0b00101000
  },
  {
    0b00000000,
    0b00011000,
    0b00111000,
    0b01110000,
    0b00110000
  }
};

void drawMazeForMemorize(Arduboy2 &arduboy) {

  int availableWidth = 128 - 58; // 70 пикселей для лабиринта
  
  // Вычисляем масштаб только для доступной ширины
  float scaleX = (float)availableWidth / (MAZE_WIDTH * CELL_SIZE);
  float scaleY = 64.0f / (MAZE_HEIGHT * CELL_SIZE);
  float scale = min(scaleX, scaleY);
  
  int scaledCellSize = max(1, (int)(CELL_SIZE * scale));
  
  // Центрируем по горизонтали в левой части (где нет UI)
  int totalWidth = MAZE_WIDTH * scaledCellSize;
  int totalHeight = MAZE_HEIGHT * scaledCellSize;
  int offsetX = (availableWidth - totalWidth) / 2; // Центрируем в левой области
  int offsetY = (64 - totalHeight) / 2;
  
  for (int x = 0; x < MAZE_WIDTH; x++) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
      int screenX = x * scaledCellSize + offsetX;
      int screenY = y * scaledCellSize + offsetY;
      
      if (scaledCellSize <= 2) {
        if (maze[x][y] != 0) {
          arduboy.drawPixel(screenX, screenY, WHITE);
          if (scaledCellSize == 2) {
            arduboy.drawPixel(screenX+1, screenY, WHITE);
            arduboy.drawPixel(screenX, screenY+1, WHITE);
            arduboy.drawPixel(screenX+1, screenY+1, WHITE);
          }
        }
      } else {
        switch (maze[x][y]) {
          case 1:
            arduboy.fillRect(screenX, screenY, scaledCellSize, scaledCellSize, WHITE);
            break;
          case 2:
            if (scaledCellSize >= 4) {
              arduboy.fillRect(screenX, screenY, scaledCellSize, scaledCellSize, WHITE);
              arduboy.fillRect(screenX+1, screenY+1, scaledCellSize-2, scaledCellSize-2, BLACK);
            } else {
              arduboy.fillRect(screenX, screenY, scaledCellSize, scaledCellSize, WHITE);
            }
            break;
          case 3:
            if (scaledCellSize >= 3) {
              arduboy.drawCircle(screenX + scaledCellSize/2, screenY + scaledCellSize/2, scaledCellSize/2-1, WHITE);
            } else {
              arduboy.drawPixel(screenX, screenY, WHITE);
            }
            break;
          case 4:
            if (scaledCellSize >= 3) {
              arduboy.drawRect(screenX, screenY, scaledCellSize, scaledCellSize, WHITE);
            } else {
              arduboy.drawPixel(screenX, screenY, WHITE);
            }
            break;
        }
      }
    }
  }
}

void drawMazeCell(Arduboy2 &arduboy, int x, int y, int size, byte cellType) {
  switch (cellType) {
    case 1: // Стена
      arduboy.fillRect(x, y, size, size, WHITE);
      break;
    case 2: // Дверь
      arduboy.fillRect(x, y, size, size, WHITE);
      arduboy.fillRect(x+1, y+1, size-2, size-2, BLACK);
      break;
    case 3: // Ключ
      arduboy.drawCircle(x + size/2, y + size/2, size/2-1, WHITE);
      break;
    case 4: // Выход
      arduboy.drawRect(x, y, size, size, WHITE);
      break;
  }
}

void drawMaze(Arduboy2 &arduboy, bool showAll, int playerX, int playerY, float zoom, const Camera& camera) {
  for (int x = 0; x < MAZE_WIDTH; x++) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
      int screenX = x * CELL_SIZE * zoom - camera.x;
      int screenY = y * CELL_SIZE * zoom - camera.y;
      int cellSize = CELL_SIZE * zoom;
      
      // Проверяем видимость клетки
      if (screenX < 128 && screenX + cellSize > 0 && 
          screenY < 64 && screenY + cellSize > 0) {
        
        if (showAll) {
          drawMazeCell(arduboy, screenX, screenY, cellSize, maze[x][y]);
        } else {
          if (abs(x - playerX) <= VIEW_RADIUS && abs(y - playerY) <= VIEW_RADIUS) {
            drawMazeCell(arduboy, screenX, screenY, cellSize, maze[x][y]);
          }
        }
      }
    }
  }
}

void drawPlayer(Arduboy2 &arduboy, int playerX, int playerY, float zoom, const Camera& camera, int skinIndex = 0) {
  int screenX = playerX * CELL_SIZE * zoom - camera.x;
  int screenY = playerY * CELL_SIZE * zoom - camera.y;
  
  // Рисуем спрайт персонажа
  for (int y = 0; y < SKIN_SIZE; y++) {
    byte row = pgm_read_byte(&playerSkins[skinIndex][y]);
    for (int x = 0; x < SKIN_SIZE; x++) {
      if (row & (1 << (7 - x))) { // Проверяем бит
        int pixelX = screenX + x;
        int pixelY = screenY + y;
        // Проверяем что пиксель в пределах экрана
        if (pixelX >= 0 && pixelX < 128 && pixelY >= 0 && pixelY < 64) {
          arduboy.drawPixel(pixelX, pixelY, WHITE);
        }
      }
    }
  }
}

void drawUI(Arduboy2 &arduboy, unsigned long startTime, int currentLevel, bool hasKey, GameState gameState) {
  int timeLeft = (GAME_TIME - (millis() - startTime)) / 1000;
  arduboy.setCursor(70, 0);
  arduboy.print("Time:");
  arduboy.print(timeLeft);
  
  arduboy.setCursor(70, 10);
  arduboy.print("Level:");
  arduboy.print(currentLevel);
  
  arduboy.setCursor(70, 20);
  arduboy.print("Key:");
  arduboy.print(hasKey ? "Y" : "N");

  arduboy.setCursor(70, 40);
  arduboy.print("B:Pause");
  
  arduboy.setCursor(70, 50);
  arduboy.print("A:Use");
}

void drawMenu(Arduboy2 &arduboy, int menuIndex, bool soundOn) {
  const char* items[3] = { "START", "TUTORIAL", soundOn ? "SOUND: ON" : "SOUND: OFF" };

  arduboy.setCursor(20, 10);
  arduboy.print("MAZE MEMORY");

  for(int i = 0; i < 3; i++) {
    arduboy.setCursor(30, 30 + i*12);
    if(i == menuIndex) arduboy.print(">"); // выделение
    arduboy.print(items[i]);
  }
  /*
  arduboy.setCursor(20, 20);
  arduboy.print("MAZE MEMORY");
  arduboy.setCursor(20, 35);
  arduboy.print("A - START");
  arduboy.setCursor(20, 45);
  arduboy.print("B - MENU"); 
  */
}

void drawWinScreen(Arduboy2 &arduboy, int currentLevel) {
  arduboy.setCursor(30, 25);
  arduboy.print("LEVEL ");
  arduboy.print(currentLevel);
  arduboy.setCursor(20, 35);
  arduboy.print("COMPLETED!");
  arduboy.setCursor(15, 50);
  arduboy.print("A - NEXT");
}

void drawGameOver(Arduboy2 &arduboy) {
  arduboy.setCursor(35, 25);
  arduboy.print("TIME UP!");
  arduboy.setCursor(20, 40);
  arduboy.print("A - MENU");
}

void drawSkinSelection(Arduboy2 &arduboy, int selectedSkin) {
  arduboy.setCursor(30, 5);
  arduboy.print("SELECT SKIN");
  
  // Рисуем 4 варианта скинов
  for (int i = 0; i < NUM_SKINS; i++) {
    int x = 20 + (i % 2) * 60;
    int y = 20 + (i / 2) * 25;
    
    // Рамка выбора
    if (i == selectedSkin) {
      arduboy.drawRect(x-2, y-2, SKIN_SIZE+4, SKIN_SIZE+4, WHITE);
    }
    
    // Рисуем спрайт персонажа
    for (int py = 0; py < SKIN_SIZE; py++) {
      byte row = pgm_read_byte(&playerSkins[i][py]);
      for (int px = 0; px < SKIN_SIZE; px++) {
        if (row & (1 << (7 - px))) {
          arduboy.drawPixel(x + px, y + py, WHITE);
        }
      }
    }
    
    // Номер скина
    arduboy.setCursor(x + SKIN_SIZE + 5, y + 2);
    arduboy.print(i + 1);
  }
  
  // Подсказки управления
  arduboy.setCursor(15, 55);
  arduboy.print("A:Select  B:Back");
}