#include "test.h"

#ifdef TEST6

#include "Audio.h"
#include ""

AudioSynthWaveformSine  sine1;
AudioSynthWaveformSine  sine2;
AudioOutputAnalog              dac1(0);
AudioOutputAnalog              dac2(1);
AudioConnection                  patchCord1(sine1, dac1);
AudioConnection                  patchCord1(sine2, dac2);

void setup (void) {
  AudioMemory(12);
  sine1.amplitude(1.0);
  sine1.frequency(100);
  sine2.amplitude(2.0);
  sine2.frequency(200);
}

void loop (void) {
}

#endif