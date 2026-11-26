#include <Arduboy2.h>
// #include <ArduboyTones.h> Звук
#include "config.h"
#include "Game.h"
#include "graphics.h"
#include "levels.h"

Arduboy2 arduboy;
// ArduboyTones tones; Звук
Game game;

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
