#include <Arduino.h>
#include "wave.h"
#include "square.h"
#include "sine.h"

void setup() {
  setupWaves();
  setupSine();
  setupSquare();
}

FASTRUN void loop() {
  generateSquare(9999, 50);
}