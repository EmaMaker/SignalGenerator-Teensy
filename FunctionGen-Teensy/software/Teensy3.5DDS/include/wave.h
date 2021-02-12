#pragma once

#include <Arduino.h>

#ifdef WAVE_H
#define extr
#else
#define extr extern
#endif

#define MAX_VALUE 4096
#define MAX_VALUE_HALF 2048

//How much time (nanoseconds) is required for an analogWriteDAC0() call
#define ANALOG_WRITE_TIME_NS 477
#define ANALOG_WRITE_TIME_MS 0.5f

// How much time a single clock cycle takes
#define CLOCK_TO_NS 9
// Overhead when counting cycles
#define CYCLE_OVERHEAD 0

FASTRUN void startGenerating();
FASTRUN void stopGenerating();
FASTRUN void delayCycles(unsigned long);

void setupWaves();

extr bool generateWave;

/* Generate values for the filters from: https://tools.analog.com/en/filterwizard/*/
