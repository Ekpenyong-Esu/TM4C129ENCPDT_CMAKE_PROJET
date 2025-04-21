// SysTickTestMain.c
// Runs on TM4C129ENCPDT
// Implements a running LED pattern across four LEDs
// Modified for TM4C129ENCPDT from original TM4C123 code

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "PLL.h"
#include "SysTick.h"

// LED pin definitions for TM4C129ENCPDT
// Two LEDs on Port N (PN0, PN1) and two on Port F (PF0, PF4)
#define LED_PORT_N      GPIO_PORTN_BASE
#define LED_PORT_F      GPIO_PORTF_BASE
#define LED_PINS_N      (GPIO_PIN_0 | GPIO_PIN_1)
#define LED_PINS_F      (GPIO_PIN_0 | GPIO_PIN_4)
#define LED_PERIPH_N    SYSCTL_PERIPH_GPION
#define LED_PERIPH_F    SYSCTL_PERIPH_GPIOF

int main(void) {
  // Enable the GPIO ports that are used for the on-board LEDs
  SysCtlPeripheralEnable(LED_PERIPH_N);
  SysCtlPeripheralEnable(LED_PERIPH_F);
  while(!SysCtlPeripheralReady(LED_PERIPH_N) || !SysCtlPeripheralReady(LED_PERIPH_F)) {}

  // Initialize the system clock to 120MHz (maximum for TM4C129ENCPDT)
  PLL_Init(Bus120MHz);
  
  // Initialize SysTick timer
  SysTick_Init();
  
  // Configure the GPIO pins for output
  GPIOPinTypeGPIOOutput(LED_PORT_N, LED_PINS_N);
  GPIOPinTypeGPIOOutput(LED_PORT_F, LED_PINS_F);
  
  // Initialize all LEDs to off state
  GPIOPinWrite(LED_PORT_N, LED_PINS_N, 0);
  GPIOPinWrite(LED_PORT_F, LED_PINS_F, 0);

  // Current LED position (0-3)
  uint8_t currentLed = 0;
  
  // Main loop - running LED pattern
  while (1) {
    // Turn off all LEDs
    GPIOPinWrite(LED_PORT_N, LED_PINS_N, 0);
    GPIOPinWrite(LED_PORT_F, LED_PINS_F, 0);
    
    // Turn on current LED based on position
    switch(currentLed) {
      case 0:
        GPIOPinWrite(LED_PORT_N, GPIO_PIN_0, GPIO_PIN_0); // First LED on Port N (PN0)
        break;
      case 1:
        GPIOPinWrite(LED_PORT_N, GPIO_PIN_1, GPIO_PIN_1); // Second LED on Port N (PN1)
        break;
      case 2:
        GPIOPinWrite(LED_PORT_F, GPIO_PIN_0, GPIO_PIN_0); // Third LED on Port F (PF0)
        break;
      case 3:
        GPIOPinWrite(LED_PORT_F, GPIO_PIN_4, GPIO_PIN_4); // Fourth LED on Port F (PF4)
        break;
    }
    
    // Move to next LED position
    currentLed = (currentLed + 1) % 4;
    
    // Wait using SysTick
    SysTick_Wait10ms(200);  // 1000ms delay for visible pattern
  }
}
