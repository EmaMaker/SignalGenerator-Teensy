#include "test.h"
#ifdef TEST1

//The analogWrite function takes an int as argument. The DAC is 12 bit. So max int value we can use is 2^16
//Precalculate it, don't use a define
int MAX_VALUE = pow(2, 12);
int MAX_VALUE_HALF = MAX_VALUE / 2;

//DAC voltage value goes from 0V (analogWrite(0)) to 3.3V (analogWrite(pow(2,12)))

//Function declaration
void sineWave();
void resetSineWave();
void sawTooth(boolean);
float sineApprox(float);

float freq = 1000; //Hertz

void setup()
{
  //Set DACs resolution to 12 bits
  analogWriteResolution(12);

  Serial.begin(9600);

  resetSineWave();
}

float amplitude = 1;

void loop()
{
  sineWave();
}

float t = 0;
float omega = freq * 2 * PI;

int oldValue = 0;
unsigned long lastTime = 0;
unsigned long time = 0;

//This is not frequency accurate. Frequency changes by changing the t value, which is totally not correct
void sineWave()
{
  int y1 = MAX_VALUE_HALF + MAX_VALUE_HALF * sin(omega + t );

  analogWriteDAC0(y1);

  t += 0.26;

}

void resetSineWave()
{
  t = 0;
}

void sawTooth(boolean delay_)
{
  //Sawthoot wave
  for (int i = 0; i < MAX_VALUE; i++)
  {
    analogWriteDAC0(i);
    analogWriteDAC1(i);
    if (delay_)
      delay(100);
  }
}

//Sine approximation function https://en.wikipedia.org/wiki/Bhaskara_I%27s_sine_approximation_formula
float sineApprox(float x)
{
  return (16 * x * (PI - x) / (5 * PI * PI - 4 * x * (PI - x)));
}
#endif