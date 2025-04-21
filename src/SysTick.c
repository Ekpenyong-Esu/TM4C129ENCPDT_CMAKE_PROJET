// SysTick.c
// Runs on TM4C129ENCPDT
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait.
// Modified for TM4C129ENCPDT which supports up to 120 MHz

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

// NVIC registers for SysTick
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))

#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void) {
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
  // enable SysTick with core clock
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC;
}

// Time delay using busy wait.
// The delay parameter is in units of the core clock
void SysTick_Wait(uint32_t delay) {
  NVIC_ST_RELOAD_R = delay - 1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;         // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0) { // wait for count flag
  }
}

// Time delay using busy wait.
// This function automatically adjusts for different clock speeds
void SysTick_Wait10ms(uint32_t delay) {
  // Get the current clock frequency - TM4C129 uses SysCtlClockGet()
  uint32_t clock = SysCtlClockGet();
  
  // Calculate the number of cycles for 10ms based on clock frequency
  uint32_t cycles = (clock / 100); // 10ms = 1/100 of a second
  
  uint32_t i;
  for(i = 0; i < delay; i++) {
    SysTick_Wait(cycles);  // wait 10ms based on current clock
  }
}

// For compatibility with TM4C123 code - these use fixed clock assumptions
void SysTick50_Wait10ms(uint32_t delay) {
  uint32_t i;
  for(i = 0; i < delay; i++) {
    SysTick_Wait(500000);  // wait 10ms (assumes 50 MHz clock)
  }
}

void SysTick80_Wait10ms(uint32_t delay) {
  uint32_t i;
  for(i = 0; i < delay; i++) {
    SysTick_Wait(800000);  // wait 10ms (assumes 80MHz clock)
  }
}

// Added for 120MHz clock of TM4C129
void SysTick120_Wait10ms(uint32_t delay) {
  uint32_t i;
  for(i = 0; i < delay; i++) {
    SysTick_Wait(1200000);  // wait 10ms (assumes 120MHz clock)
  }
}

uint32_t Time;
// start the stopwatch
void SysTick_Start(void) {
  Time = NVIC_ST_CURRENT_R;
}

// end the stopwatch, and return elapsed time in bus cycles
uint32_t SysTick_Stop(void) {
  return (Time - NVIC_ST_CURRENT_R) & 0x00FFFFFF;
}
