#pragma once

#define LUT_SIZE 512

// Sampling frequency of the DAC
// Since the teensy runs on 168MHz and Fastest + LTO, using the DAC sampling frequency (1428571Hz) from the Datasheet kinda falls apart
#define SAMPLE_FREQ 6222000 

//Max Freq with good results achievable with Teensy 3.5 DAC
#define MAX_ACHIEVABLE_FREQ 1000000

void generateSine(float);
void setupSine();