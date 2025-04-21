// PLL.c
// Runs on TM4C129ENCPDT
// A software function to change the bus frequency using the PLL.
// Modified for TM4C129ENCPDT which has different PLL configuration
// than the TM4C123 series.

#include <stdint.h>
#include <stdbool.h>
#include "PLL.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"

// The System Clock Options for TM4C129ENCPDT
#define CRYSTAL_FREQ        25000000  // 25MHz crystal (common on TM4C129 boards)
#define BUS_FREQ            120000000 // 120MHz maximum clock for TM4C129

// configure the system to get its clock from the PLL
// freq is bus frequency in Hz
void PLL_Init(uint32_t freq) {
    // For TM4C129, we'll use the newer SysCtl functions from driverlib
    // since the register interface is different than TM4C123
    
    uint32_t osc_source = SYSCTL_OSC_MAIN;  // Use the main oscillator
    uint32_t xtal = SYSCTL_XTAL_25MHZ;      // 25MHz crystal on many TM4C129 boards
    uint32_t config = SYSCTL_USE_PLL;       // Use PLL
    
    switch(freq) {
        case Bus120MHz:
            SysCtlClockFreqSet(osc_source | xtal | config, 120000000);
            break;
        case Bus100MHz:
            SysCtlClockFreqSet(osc_source | xtal | config, 100000000);
            break;
        case Bus80MHz:
            SysCtlClockFreqSet(osc_source | xtal | config, 80000000);
            break;
        case Bus50MHz:
            SysCtlClockFreqSet(osc_source | xtal | config, 50000000);
            break;
        case Bus40MHz:
            SysCtlClockFreqSet(osc_source | xtal | config, 40000000);
            break;
        case Bus25MHz:
            SysCtlClockFreqSet(osc_source | xtal | SYSCTL_USE_OSC, 25000000);
            break;
        default:
            // Default to 80MHz if invalid frequency requested
            SysCtlClockFreqSet(osc_source | xtal | config, 80000000);
            break;
    }
}
