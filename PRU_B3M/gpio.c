

#include "gpio.h"

volatile register uint32_t __R30;

/************************************************
 * initGPIO
 * Initilise the GPIO module.
 * \param gpio: chosen GPIO
 ************************************************/ 
void initGPIO() {
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
}


/************************************************
 * toggleGPIO
 * toggle the selected GPIO pin.
 * \param gpio: chosen GPIO
 ************************************************/ 
void toggleGPIO(uint32_t gpio) {
	__R30 ^= gpio;
}

/************************************************
 * resetGPIO
 * Set the selected GPIO pin (i.e., set to high).
 * \param gpio: chosen GPIO
 ************************************************/ 
void setGPIO(uint32_t gpio) {
	__R30 |= gpio;
}


/************************************************
 * resetGPIO
 * Reset the selected GPIO pin (i.e., set to low).
 * \param gpio: chosen GPIO
 ************************************************/ 
void clearGPIO(uint32_t gpio) {
	__R30 &= ~gpio;
}


/************************************************
 * debug_lights
 * Toggle the GPIO P9.27 for cnt number of times
 * \param cnt: No. of times to toggle the GPIO
 ************************************************/ 
void debug_lights(uint8_t cnt) {
	uint8_t i;

	for(i=0; i<cnt; i++){
		__R30 |= (1U << 5); // Set the pin to high; P9.27
		__delay_cycles(100);
		__R30 &= ~(1U << 5); // Clear the pin; P9.27
		__delay_cycles(100);
	}

	return;
}
