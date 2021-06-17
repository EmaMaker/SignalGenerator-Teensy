#pragma once

#include <Arduino.h>

#ifdef WAVE_H
#define extr
#else
#define extr extern
#endif

#define MAX_VALUE 512
#define MAX_VALUE_HALF 256

//How much time (nanoseconds) is required for an analogWriteDAC0() call
#define ANALOG_WRITE_TIME_NS 477
#define ANALOG_WRITE_TIME_MS 0.5f

// How much time a single clock cycle takes
#define CLOCK_TO_NS 9
// Overhead when counting cycles
#define CYCLE_OVERHEAD 12

#define DAC1_SEL 23
#define DAC0_SEL 22

#define LUT_SIZE 512

// Sampling frequency of the DAC
// Since the teensy runs on 168MHz and Fastest + LTO, using the DAC sampling frequency (1428571Hz) from the Datasheet kinda falls apart
#define SAMPLE_FREQ 6988800 

//Max Freq with good results achievable with Teensy 3.5 DAC
#define MAX_ACHIEVABLE_FREQ 1000000

void calculateSineLookup();
void calculateSquareLookup(int);
void generateSine(float);
void generateSquare(float, int);
void generateWave(float, int*, int);

void noSelect();
void selectDAC0();
void selectDAC1();

FASTRUN void startGenerating();
FASTRUN void stopGenerating();
FASTRUN void generateWave();
FASTRUN void delayCycles(unsigned long);

void setupWaves();

extr bool generating;

/* Generate values for the filters from: https://tools.analog.com/en/filterwizard/*/
