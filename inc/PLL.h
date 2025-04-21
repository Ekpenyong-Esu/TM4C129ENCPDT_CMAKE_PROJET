// PLL.h
// Runs on TM4C129ENCPDT
// A software function to change the bus frequency using the PLL.
// Modified for TM4C129 series microcontrollers

#include <stdint.h>

// The TM4C129ENCPDT supports up to 120MHz operation
// These definitions are used to specify the desired bus frequency

// configure the system to get its clock from the PLL
void PLL_Init(uint32_t freq);

// New frequency options for TM4C129ENCPDT
#define Bus120MHz    0  // 120 MHz (maximum for TM4C129)
#define Bus100MHz    1  // 100 MHz
#define Bus80MHz     2  // 80 MHz
#define Bus50MHz     3  // 50 MHz
#define Bus40MHz     4  // 40 MHz 
#define Bus25MHz     5  // 25 MHz (bypass PLL and use crystal directly)

// Legacy frequency options for compatibility with TM4C123 code
// These will be mapped to appropriate TM4C129 frequencies
#define Bus80_000MHz Bus80MHz
#define Bus66_667MHz Bus80MHz
#define Bus50_000MHz Bus50MHz
#define Bus44_444MHz Bus40MHz
#define Bus40_000MHz Bus40MHz
#define Bus36_364MHz Bus40MHz
#define Bus33_333MHz Bus40MHz
#define Bus30_769MHz Bus25MHz
#define Bus28_571MHz Bus25MHz
#define Bus26_667MHz Bus25MHz
#define Bus25_000MHz Bus25MHz




