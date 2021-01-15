#include "test.h"

#ifdef TEST3
//Lookup table test

//Use a lookup table to generate the sine wave. This works well and generates a good 0-3.3V sine wave
//After playing around with the sampling freq, this is good enough but still has some artifacts on the wave, probably due to the phase accumulator wraparound. But definitely the way to go here
//Above a certain value (like 130khz) linear interpolation is NEEDED to smooth out the wave, probably due to the sampling rate of the DAC

//Next-day note: the artifacts on the wave definetely were because of the phase wraparound, changing the way the DAC was driven and the phase accumulated this now works fine
//Linear interpolation is still needed though 

//The analogWrite function takes an int as argument. The DAC is 12 bit. So max int value we can use is 2^12
//Precalculate it, don't use a define

int MAX_VALUE = pow(2, 12);
int MAX_VALUE_HALF = MAX_VALUE / 2;

//How many samples of the wave we want
#define LUT_SIZE 1024
int LUT[LUT_SIZE];

const int BUFF_SIZE = 1024; // size of output buffer (samples)
int16_t buff[BUFF_SIZE];    // output buffer

//DAC voltage value goes from 0V (analogWrite(0)) to 3.3V (analogWrite(pow(2,12)))

void calculateLookupTable();

FASTRUN void setup() {
  //Set DACs resolution to 12 bits
  analogWriteResolution(12);

  //Disable interrupts for extra speed
  noInterrupts();

  calculateLookupTable();
}

long sampleFreq = 1818181; //DAC sampling time(1428571,428571429), then i played around with it

const int f = 50000;                                      // frequency we want to generate (Hz)
const float delta_phi = (float)f / sampleFreq * LUT_SIZE; // phase increment
int phase = 0.0f;                                       // phase accumulator

FASTRUN void loop() {

  // generate buffer of output
  for (int i = 0; i < BUFF_SIZE; ++i) {
    buff[i] = LUT[phase];       // get sample value from LUT
    phase = (phase + delta_phi) % LUT_SIZE;           // increment phase
    if (phase >= (float)LUT_SIZE) // handle wraparound
      phase -= (float)LUT_SIZE;
      
    analogWriteDAC0(buff[i]); // write the selected waveform on DAC
  }
}

//Using the analitic formula for a sine wave, calculate the lookup table
void calculateLookupTable() {
  for (int i = 0; i < LUT_SIZE; i++) {
    LUT[i] = MAX_VALUE_HALF + MAX_VALUE_HALF * sin((2 * PI * (float)i) / LUT_SIZE);
  }
}
#endif