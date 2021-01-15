#include "test.h"

#ifdef TEST4
//Lookup table test

//Use a lookup table to generate a quarter of a sine sine wave. This eliminates asimmetries and random frequency shiftings (very slight but noticeable enough)
//It is also a faster algo
//This however has some problems in the zones when a quarter if shifted, giving some artifacts that point to MAX_VALUE_HALF, like if there was MAX_VALUE_HALF in the table at that point

//The analogWrite function takes an int as argument. The DAC is 12 bit. So max int value we can use is 2^12
//Precalculate it, don't use a define

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

  Serial.begin(115200);
}

long sampleFreq = 1428571; //DAC sampling time(1428571,428571429), then i played around with it

const int f = 10000;                                      // frequency we want to generate (Hz)
const float delta_phi = (float)f / sampleFreq * (LUT_SIZE*4); // phase increment
int phase = 0.0f;                                       // phase accumulator

int LUT_SIZE_QUARTER = LUT_SIZE * 2;
int LUT_SIZE_HALF = LUT_SIZE * 3;
int LUT_SIZE_3QUARTERS = LUT_SIZE * 4;

FASTRUN void loop() {

  // generate buffer of output
  for (int i = 0; i < BUFF_SIZE; ++i) {
    
    phase += delta_phi;           // increment phase

    int phase_i = (int)phase;     // get integer part of our phase

    //For the first quarter of the lut, generate a quarter of a sine wave

    if(phase >= 0 && phase < LUT_SIZE) buff[i] = LUT[phase_i];
    else if(phase >= LUT_SIZE && phase < LUT_SIZE*2) buff[i] = LUT[(2*LUT_SIZE-phase_i)%LUT_SIZE];
    else if(phase >= LUT_SIZE*2 && phase < LUT_SIZE*3) buff[i] = MAX_VALUE-LUT[(2*LUT_SIZE+phase_i)%LUT_SIZE];
    else if(phase >= LUT_SIZE*3 && phase <= LUT_SIZE*4) buff[i] = MAX_VALUE-LUT[(4*LUT_SIZE-phase_i)%LUT_SIZE];
    else if (phase > (float)LUT_SIZE*4) phase -= (float)(LUT_SIZE*4); // handle wraparound

    //This is a very ugly fix to the spikes that sometimes happed when changing quadrant
    // if (abs(oldValue-buff[i]) >= MAX_VALUE_HALF*0.65) buff[i] = oldValue;
    // else oldValue = buff[i]
    

    analogWriteDAC0(buff[i]);
  }
}

//Calculate the lookup table for a quarter (pi/2) of the sinewave
void calculateLookupTable() {
  for (int i = 0; i < LUT_SIZE; i++) {
    LUT[i] = MAX_VALUE_HALF + MAX_VALUE_HALF * sin((PI/2) * i / LUT_SIZE);
  }
}
#endif