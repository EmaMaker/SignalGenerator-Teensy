#include "test.h"

#ifdef TEST2
//Lookup table test

//Use a lookup table to generate the sine wave. This works well and generates a good 0-3.3V sine wave
//Frequency is modulated using "samplerate", which waits a defined time between outputting a point and another.
//The digital oscilloscope reports the frequency to be in a range 10-100 hertz from the defined requency (drifting higher the higher the frequency)
//Frequency caps at 1.5kHz, but it's stabler the lower it goes
//delay functions in teensy take long as arguments, eliminating the decimal part and giving us less numbers to use (samplerate becomes lower and lower the higher the frequency)
//Approximation errors errors cap the frequency and make it less accurate

//The analogWrite function takes an int as argument. The DAC is 12 bit. So max int value we can use is 2^12
//Precalculate it, don't use a define

int MAX_VALUE = pow(2,  12);
int MAX_VALUE_HALF = MAX_VALUE / 2;

//DAC voltage value goes from 0V (analogWrite(0)) to 3.3V (analogWrite(pow(2,12)))

volatile int i = 0;

float freq = 100; //Hertz

FASTRUN void setup() {
  //Set DACs resolution to 12 bits
  analogWriteResolution(12);
  
  //Disable interrupts for extra speed
  noInterrupts();
}

//This is how much time can pass between two points
//We wait micros (10^-6) to mult by 10^6
float samplerate = ((1/freq)/maxSamplesNum)*pow(10, 6);

FASTRUN void loop() {
    analogWriteDAC0(waveformsTable[i]);  // write the selected waveform on DAC
    i = (i+1)%maxSamplesNum;

    //Using delay instead of micros() produces defined waves that don't wobble because of the approximation errors, since delayMicroseconds takes a long and not a float as argument
    delayMicroseconds(samplerate);
}
#endif