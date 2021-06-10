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

FASTRUN void startGenerating();
FASTRUN void stopGenerating();
FASTRUN void delayCycles(unsigned long);

void setupWaves();

extr bool generateWave;

/* Generate values for the filters from: https://tools.analog.com/en/filterwizard/*/
