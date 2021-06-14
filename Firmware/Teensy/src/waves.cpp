#define WAVE_H

#include "Arduino.h"
#include "waves.h"

int SINE_LUT[LUT_SIZE];
int SQUARE_LUT[LUT_SIZE];

void setupWaves(){
  pinMode(DAC0_SEL, OUTPUT);
  pinMode(DAC1_SEL, OUTPUT);

  // Info from teensy forum, access cycle counter. This will be needed for square waves in particular
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
  
  //Set DACs resolution to 12 bits
  analogWriteResolution(12);

  // Enable DAC0 and DAC1, from framework's analogWriteDAC0/DAC1, place here from higher speeds
  // Reference voltage is 1.2V
  SIM_SCGC2 |= SIM_SCGC2_DAC0;
	DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;
	SIM_SCGC2 |= SIM_SCGC2_DAC1;
  DAC1_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;

  noSelect();

  //This one can be calculated only once
  calculateSineLookup();
}

// Wait time by counting clock cycles
FASTRUN void delayCycles(unsigned long t){
  unsigned long begin = ARM_DWT_CYCCNT-CYCLE_OVERHEAD;
  while(ARM_DWT_CYCCNT - begin < t) { ; } //wait
}

// This needs to be called from the interrupt triggered by the rotary encoder being pressed
void stopGenerating(){
    generating = false;
}

FASTRUN void startGenerating(){
    generating = true;
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
    if(i >= 0 && i <= LUT_SIZE_QUARTER) SINE_LUT[i] = MAX_VALUE_HALF + (MAX_VALUE_HALF-1) * sin((2 * PI * (float)i) / LUT_SIZE);
    else if(i > LUT_SIZE_QUARTER && i <= LUT_SIZE_HALF) SINE_LUT[i] = SINE_LUT[(LUT_SIZE_HALF-i)];
    else SINE_LUT[i] = MAX_VALUE-SINE_LUT[(LUT_SIZE-i)];;
  }
}

void calculateSquareLookup(int duty){
  int i_duty = LUT_SIZE*0.01*duty;
  for(int i = 0; i < LUT_SIZE; i++) SQUARE_LUT[i] = MAX_VALUE*(i<i_duty);
}

void selectDAC0(){
  digitalWriteFast(DAC0_SEL, HIGH);
  digitalWriteFast(DAC1_SEL, LOW);  
}

void selectDAC1(){
  digitalWriteFast(DAC0_SEL, LOW);
  digitalWriteFast(DAC1_SEL, HIGH);  
}

void noSelect(){
  digitalWriteFast(DAC0_SEL, LOW);
  digitalWriteFast(DAC1_SEL, LOW);  
}

//Write the DAC with using some parts of framework's Arduino.h, for higher speeds
// Use FASTRUN to run code in RAM
typedef int16_t __attribute__((__may_alias__)) aliased_int16_t;
FASTRUN void generateWave(float frequency, int LUT[], int dac){
    startGenerating();
    
    // Phase accumulator
    float phase = 0;

    //Constrain frequency
    frequency = constrain(frequency, 0, MAX_ACHIEVABLE_FREQ);

    // Phase increment at each step
    float delta_phi = frequency / SAMPLE_FREQ * LUT_SIZE; 
    
    while(generating){
        // increment phase
        phase += delta_phi;
        
        // handle wraparound           
        if (phase >= LUT_SIZE) phase -= LUT_SIZE;
        
        //Write the voltage on the DAC
        if(dac == 0) *(volatile aliased_int16_t *)&(DAC0_DAT0L) = LUT[(int)phase];
        else *(volatile aliased_int16_t *)&(DAC1_DAT0L) = LUT[(int)phase];
    }
}

FASTRUN void generateSine(float frequency){
  selectDAC0();
  generateWave(frequency, SINE_LUT, 0);
}

FASTRUN void generateSquare(float frequency, int duty){
    selectDAC1();
    calculateSquareLookup(duty);
    generateWave(frequency, SQUARE_LUT, 1);
}