#include "wave.h"
#include "sine.h"

void calculateSineLookup();

int LUT[LUT_SIZE];

void setupSine(){
    calculateSineLookup();
}

/* Calculate a sine lookup table using the analytic formula for a sine wave y=A*sin(wt+phi)
Only the first quarter (quadrant) of the wave is actually getting calculated. This saves up time for calculation
and assures the simmetry of the wave in each quadrant
*/
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

//Write the DAC with using some parts of framework's Arduino.h, for higher speeds
// Use FASTRUN to run code in RAM
typedef int16_t __attribute__((__may_alias__)) aliased_int16_t;

FASTRUN void generateSine(float frequency){
    startGenerating();

    // Phase accumulator
    float phase = 0;

    //Constrain frequency
    frequency = constrain(frequency, 0, MAX_ACHIEVABLE_FREQ);

    // Phase increment at each step
    float delta_phi = frequency / SAMPLE_FREQ * LUT_SIZE; 
    
    while(generateWave){
        // increment phase
        phase += delta_phi;
        
        // handle wraparound           
        if (phase >= LUT_SIZE) phase -= LUT_SIZE;
        
        //Write the voltage on the DAC
        *(volatile aliased_int16_t *)&(DAC0_DAT0L) = LUT[(int)phase];
    }
}
//Write the DAC with using some parts of framework's Arduino.h, for higher speeds
// Use FASTRUN to run code in RAM
typedef int16_t __attribute__((__may_alias__)) aliased_int16_t;

FASTRUN void generateSines(float frequency1, float frequency2){
    startGenerating();

    // Phase accumulator
    float phase1 = 0;
    float phase2 = 0;

    //Constrain frequency
    frequency1 = constrain(frequency1, 0, MAX_ACHIEVABLE_FREQ);
    frequency2 = constrain(frequency2, 0, MAX_ACHIEVABLE_FREQ);

    // Phase increment at each step
    float delta_phi1 = frequency1 / SAMPLE_FREQ * LUT_SIZE; 
    float delta_phi2 = frequency2 / SAMPLE_FREQ * LUT_SIZE; 
    
    while(generateWave){
        // increment phase
        phase1 += delta_phi1;
        phase2 += delta_phi2;
        
        // handle wraparound           
        if (phase1 >= LUT_SIZE) phase1 -= LUT_SIZE;
        if (phase2 >= LUT_SIZE) phase2 -= LUT_SIZE;
        
        //Write the voltage on the DAC
        *(volatile aliased_int16_t *)&(DAC0_DAT0L) = LUT[(int)phase1];
        *(volatile aliased_int16_t *)&(DAC1_DAT0L) = LUT[(int)phase2];
    }
}