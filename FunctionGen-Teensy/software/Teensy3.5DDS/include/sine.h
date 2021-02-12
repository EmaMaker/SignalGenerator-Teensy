#pragma once

#define LUT_SIZE 4096
//DAC sampling time(1428571,428571429), found on the Datasheet
#define SAMPLE_FREQ 1428571

//Max Freq with good results achievable with Teensy 3.5 DAC
#define MAX_ACHIEVABLE_FREQ 300000

void generateSine(float);
void setupSine();