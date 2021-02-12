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

/*Generate sine wave at the given frequency
Use the classical DDS Phase Accumulator technique
On some lower frequency the maths for the phase accumulator doesn't work (delta_phi becomes too close to zero to be noticed)
and a simpler approach waiting microseconds can be used*/

// Use FASTRUN to run code in RAM
FASTRUN void generateSine(float frequency){
    startGenerating();

    if (frequency <= 500){

      // Period of the wave in microseconds
      unsigned long periodMS = 1/frequency * pow (10, 6);

      //Time to pass between outputting each sample
      unsigned long sampleTime = (unsigned long) ((float)periodMS / LUT_SIZE + ANALOG_WRITE_TIME_MS);

      int i = 0;
      while(generateWave){
        //Write the voltage on the DAC. i gets incremented after the call to the function is done
        analogWriteDAC0(LUT[i++]);

        // handle wraparound
        if(i >= LUT_SIZE) i -= LUT_SIZE;

        // Wait the needed time
        delayMicroseconds(sampleTime);
      }

    }else{
      // Phase accumulator
      float phase = 0;

      //Constrain frequency
      frequency = constrain(frequency, 0, MAX_ACHIEVABLE_FREQ);

      // Phase increment at each step
      float delta_phi = (int) (frequency / SAMPLE_FREQ * LUT_SIZE); 
      
      while(generateWave){
          // increment phase
          phase += delta_phi;
          
          // handle wraparound           
          if (phase >= (float)LUT_SIZE) phase -= (float)LUT_SIZE;
          
          //Write the voltage on the DAC
          analogWriteDAC0(LUT[(int)phase]); 
      }
    }
}