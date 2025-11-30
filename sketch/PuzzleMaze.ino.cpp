#include <Arduino.h>
#line 1 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\PuzzleMaze.ino"
#include <Arduboy2.h>
// #include <ArduboyTones.h> Звук
#include "config.h" 
#include "game.h"
#include "graphics.h"
#include "levels.h"

Arduboy2 arduboy;
// ArduboyTones tones; Звук
Game game;

#line 12 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\PuzzleMaze.ino"
void setup();
#line 19 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\PuzzleMaze.ino"
void loop();
#line 12 "C:\\Users\\TomPro\\Documents\\Arduino\\PuzzleMaze\\PuzzleMaze.ino"
void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.clear();
  arduboy.display();
}

void loop() {
  if (!arduboy.nextFrame()) return;
  
  arduboy.pollButtons();
  arduboy.clear();

  game.update(arduboy);
  game.draw(arduboy);

  arduboy.display();
}
