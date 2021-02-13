#define WAVE_H

#include "Arduino.h"
#include "wave.h"

void setupWaves(){
  // Info from teensy forum, access cycle counter. This will be needed for square waves in particular
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
  
  //Set DACs resolution to 12 bits
  analogWriteResolution(12);

  // Enable DAC0, from framework's analogWriteDAC0, place here from higher speeds
  SIM_SCGC2 |= SIM_SCGC2_DAC0;
	DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;
}

// Wait time by counting clock cycles
FASTRUN void delayCycles(unsigned long t){
  unsigned long begin = ARM_DWT_CYCCNT-CYCLE_OVERHEAD;
  while(ARM_DWT_CYCCNT - begin < t) { ; } //wait
}

// This needs to be called from the interrupt triggered by the rotary encoder being pressed
void stopGenerating(){
    generateWave = false;
}

FASTRUN void startGenerating(){
    generateWave = true;
}