#include "square.h"
#include "wave.h"

void setupSquare(){
  pinMode(SQUARE_PIN, OUTPUT);
}

/*Generate a square wave by toggling a digital HIGH and LOW at the correct speed
Count in nano seconds and wait using CPU Cycles for the maximum accuracy on higher frequency waves
Lower frequency waves using nanoseconds is not needed and could actually causes overflows due to the length of the number used, so microseconds can be used instead*/
// Use FASTRUN to run code in RAM
FASTRUN void generateSquare(float frequency, int duty){

    digitalWriteFast(DAC0_SEL, LOW); 
    digitalWriteFast(DAC1_SEL, HIGH); 

    startGenerating();
    if(frequency < 100000){
      unsigned long periodMS = (1/ frequency) * pow(10, 6);      
      unsigned long dutyHigh = periodMS * duty * 0.01;
      unsigned long dutyLow = periodMS * duty * 0.01;

      while(generateWave){
          digitalWriteFast(SQUARE_PIN, HIGH);
          delayMicroseconds(dutyHigh);
          digitalWriteFast(SQUARE_PIN, LOW);
          delayMicroseconds(dutyLow);
      }

    }else{
      unsigned long periodNS = (1/ frequency) * pow(10, 9);

      //Duration of the logic HIGH level
      unsigned long dutyHigh = periodNS * duty * 0.01;
      //Duration of the logic LOW level
      unsigned long dutyLow = periodNS - dutyHigh;

      unsigned long dutyHighCycles = (unsigned long) (dutyHigh/CLOCK_TO_NS+0.5f);
      unsigned long dutyLowCycles = (unsigned long) (dutyLow/CLOCK_TO_NS+0.5f);

      while(generateWave){
          digitalWriteFast(SQUARE_PIN, HIGH);
          delayCycles(dutyHighCycles);
          digitalWriteFast(SQUARE_PIN, LOW);
          delayCycles(dutyLowCycles);
      }
    }
}

FASTRUN void generateSquareDAC1(float frequency, int duty){
    
    digitalWriteFast(DAC0_SEL, LOW); 
    digitalWriteFast(DAC1_SEL, HIGH); 

    startGenerating();
    if(frequency <= 50000){
      unsigned long periodMS = (1/ frequency) * pow(10, 6);      
      unsigned long dutyHigh = periodMS * duty * 0.01;
      unsigned long dutyLow = periodMS * duty * 0.01;

      while(generateWave){
          analogWriteDAC1(MAX_VALUE);
          delayMicroseconds(dutyHigh);
          analogWriteDAC1(0);
          delayMicroseconds(dutyLow);
      }

    }else{
      unsigned long periodNS = (1/ frequency) * pow(10, 9);

      //Duration of the logic HIGH level
      unsigned long dutyHigh = periodNS * duty * 0.01;
      //Duration of the logic LOW level
      unsigned long dutyLow = periodNS - dutyHigh;

      unsigned long dutyHighCycles = (unsigned long) (dutyHigh/CLOCK_TO_NS+0.5f);
      unsigned long dutyLowCycles = (unsigned long) (dutyLow/CLOCK_TO_NS+0.5f);

      while(generateWave){
          analogWriteDAC1(MAX_VALUE);
          delayCycles(dutyHighCycles);
          analogWriteDAC1(0);
          delayCycles(dutyLowCycles);
      }
    }
}