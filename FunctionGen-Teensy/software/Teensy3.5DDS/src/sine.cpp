#include "wave.h"
#include "sine.h"

void calculateSineLookup();

int LUT[LUT_SIZE];

void setupSine(){
    calculateSineLookup();
}

void calculateSineLookup(){
  int LUT_SIZE_QUARTER = LUT_SIZE * 0.25;
  int LUT_SIZE_HALF = LUT_SIZE * 0.5;
  for (int i = 0; i < LUT_SIZE; i++) {
    //For the first quarter of the lut, generate a quarter of a sine wave
    if(i >= 0 && i <= LUT_SIZE_QUARTER) LUT[i] = MAX_VALUE_HALF + (MAX_VALUE_HALF-1) * sin((2 * PI * (float)i) / LUT_SIZE);
    else if(i > LUT_SIZE_QUARTER && i <= LUT_SIZE_HALF) LUT[i] = LUT[(LUT_SIZE_HALF-i)];
    else LUT[i] = MAX_VALUE-LUT[(LUT_SIZE-i)];;
  }
}

// Use FASTRUN to run code in RAM
FASTRUN void generateSine(int frequency){
    startGenerating();

    // Phase accumulator
    int phase = 0;

    frequency = frequency <= 0 ? 0 : frequency;
    frequency = frequency >= MAX_ACHIEVABLE_FREQ ? MAX_ACHIEVABLE_FREQ : frequency;

    // Phase increment
    int delta_phi = (int) ((float)frequency / SAMPLE_FREQ * LUT_SIZE); 
    
    while(generateWave){
        // increment phase
        phase += delta_phi;
        
        // handle wraparound           
        if (phase >= LUT_SIZE)
            phase -= LUT_SIZE;
        
        // write the selected waveform on DAC
        analogWriteDAC0(LUT[phase]); 
    }
}