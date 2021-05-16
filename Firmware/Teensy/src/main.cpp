#include <Arduino.h>
#include "wave.h"
#include "square.h"
#include "sine.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);


  setupWaves();
  setupSine();
  setupSquare();
}

FASTRUN void loop() {
  generateSquareDAC1(1000000, 50);
}