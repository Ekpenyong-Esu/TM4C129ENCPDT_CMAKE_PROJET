// SysTick.h
// Runs on TM4C129ENCPDT
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait.
// Modified for TM4C129ENCPDT which supports up to 120 MHz

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);

// Time delay using busy wait.
// The delay parameter is in units of the core clock.
void SysTick_Wait(uint32_t delay);

// Time delay using busy wait.
// This function automatically adapts to the system clock frequency
void SysTick_Wait10ms(uint32_t delay);

// Legacy compatibility functions with fixed clock assumptions
// Time delay using busy wait (50 MHz clock)
void SysTick50_Wait10ms(uint32_t delay);

// Time delay using busy wait (80 MHz clock)
void SysTick80_Wait10ms(uint32_t delay);

// New function for TM4C129 - Time delay using busy wait (120 MHz clock)
void SysTick120_Wait10ms(uint32_t delay);

// start the stopwatch
void SysTick_Start(void);

// end the stopwatch, and return elapsed time in bus cycles
uint32_t SysTick_Stop(void);
