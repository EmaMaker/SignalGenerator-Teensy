#include "test.h"

#ifdef TEST5
//Lookup table test

//Generate a full lookup table by using simmetries of the first quarter. Works fine, i'd use this as the base for the complete project.
//Usual artifacts from 100kHz on, but i think it just needs to switch from analogWrite to the use of registers, to drive the DAC as fast as possible.

int MAX_VALUE = pow(2, 12);
int MAX_VALUE_HALF = MAX_VALUE / 2;

//How many samples of the wave we want
#define LUT_SIZE 1024
int LUT[LUT_SIZE];

const int BUFF_SIZE = 4096; // size of output buffer (samples)
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

long sampleFreq = 1817571; //DAC sampling time(1428571,428571429), then i played around with it

const int f = 250075;                                      // frequency we want to generate (Hz)
const float delta_phi = (float)f / sampleFreq * (LUT_SIZE); // phase increment
float phase = 0.0f;                                       // phase accumulator

int oldValue;

FASTRUN void loop() {

  // generate buffer of output
  for (int i = 0; i < BUFF_SIZE; ++i) {
    int phase_i = (int)phase;     // get integer part of our phase
    buff[i] = LUT[phase_i];       // get sample value from LUT
    phase += delta_phi;           // increment phase
    if (phase >= (float)LUT_SIZE) // handle wraparound
      phase -= (float)LUT_SIZE;
      
    analogWriteDAC0(buff[i]); // write the selected waveform on DAC
  }
}

//Calculate a quarter of a sine wave, the other three quadrants are simmetris of it
void calculateLookupTable() {
  int LUT_SIZE_QUARTER = LUT_SIZE * 0.25;
  int LUT_SIZE_HALF = LUT_SIZE * 0.5;
  for (int i = 0; i < LUT_SIZE; i++) {
    //For the first quarter of the lut, generate a quarter of a sine wave
    if(i >= 0 && i <= LUT_SIZE_QUARTER) LUT[i] = MAX_VALUE_HALF + MAX_VALUE_HALF * sin((2 * PI * (float)i) / LUT_SIZE);
    else if(i > LUT_SIZE_QUARTER && i <= LUT_SIZE_HALF) LUT[i] = LUT[(LUT_SIZE_HALF-i)];
    else LUT[i] = MAX_VALUE-LUT[(LUT_SIZE-i)];;
  }
}


#endif