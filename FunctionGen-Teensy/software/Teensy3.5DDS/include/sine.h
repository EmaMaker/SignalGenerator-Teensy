#pragma once

#define LUT_SIZE 1024
//DAC sampling time(1428571,428571429), then i played around with it
#define SAMPLE_FREQ 1928571

//Max Freq with good results achievable with Teensy 3.5 DAC
#define MAX_ACHIEVABLE_FREQ 300000

void generateSine(int);
void setupSine();