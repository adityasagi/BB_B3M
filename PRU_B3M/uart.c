#include "uart.h"
#include "gpio.h"
#include "timer.h"

/**********************
 *        UART
 *********************/

/************************************************
 * UARTInit
 * Initialise the hardware UART on the PRU.
 ************************************************/ 
void UARTInit() {
	
	initGPIO();

	/* Set up UART to function at 1500000 baud - DLL divisor is 8 at 16x oversample
	 * 192MHz / 8 / 16 = 1500000 */
	CT_UART.DLL = 8;
	CT_UART.DLH = 0;
	CT_UART.MDR = 0x0;

	/* Enable Interrupts in UART module. This allows the main thread to poll for
	 * Receive Data Available and Transmit Holding Register Empty */
	CT_UART.IER = 0x7;

	/* If FIFOs are to be used, select desired trigger level and enable
	 * FIFOs by writing to FCR. FIFOEN bit in FCR must be set first before
	 * other bits are configured */
	/* Enable FIFOs for now at 1-byte, and flush them */
	CT_UART.FCR = (0x8) | (0x4) | (0x2) | (0x1);
	//CT_UART.FCR = (0x80) | (0x4) | (0x2) | (0x01); // 8-byte RX FIFO trigger

	/* Choose desired protocol settings by writing to LCR */
	/* 8-bit word, 1 stop bit, no parity, no break control and no divisor latch */
	CT_UART.LCR = 0x3;

	/* Do not enable loopback */
	CT_UART.MCR = 0x00; //0x10;

	/* Choose desired response to emulation suspend events by configuring
	 * FREE bit and enable UART by setting UTRST and URRST in PWREMU_MGMT */
	/* Allow UART to run free, enable UART TX/RX */
	CT_UART.PWREMU_MGMT = 0x6001;

	initGPIO();

}


/************************************************
 * UARTSend
 * Send data over the hardware UART in the PRU.
 * Blocking call until all data elements are transmitted.
 * \param buffer : pointer to the data buffer
 * \param count  : number of bytes to be transmitted
 * \return : The number of bytes transmitted
 ************************************************/ 
uint8_t UARTSend(uint8_t *buffer, uint8_t count) {

	uint8_t cnt;

	setGPIO(EN_PIN); // Set the EN pin HIGH

	/* Send data */
	for (cnt = 0; cnt < count; cnt++) {
	
		CT_UART.THR = buffer[cnt]; // Write to FIFO

		/* Wait for TX FIFO and the shift register to be empty */
		while (!((CT_UART.LSR & 0x40) == 0x40));
	}

#if 0
	// Write a dummy byte to allow for the GPIO to sync with the UART
	// shift register. This will help get rid of the wait.
	CT_UART.THR = 0xFF; // write a dummy byte 0xFF
	/* Wait for TX FIFO to be empty */
	while (!((CT_UART.FCR & 0x2) == 0x2));
#endif
	//wait(185);// TODO: Remove this delay once the sync problem is fixed

	clearGPIO(EN_PIN); // Clear the EN pin to low

	return cnt;
}

/************************************************
 * UARTReceive
 * Receive data over the hardware UART in the PRU.
 * Receives a max of count data elements.
 * Waits indefinitely for the first byte of data.
 * Blocking call, waits for atleast count number of characters
 * before a timeout occurs.
 * \param buffer  : pointer to the data buffer
 * \param count   : max number of bytes to be received
 * \param timeout : timeout in microseconds between characters
 * \return        : number of bytes received
 ************************************************/ 
uint8_t UARTReceive(uint8_t *buffer, uint8_t count, uint32_t timeout) {

	uint8_t cnt;
	uint32_t time_cnt;

	// Wait indefinitely for the first byte
	while((CT_UART.LSR & 0x1) == 0x0){}

	/* Let's send/receive some dummy data */
	for (cnt = 0; cnt < count; cnt++) {

		time_cnt=timeout*200; // usec to 5 nsec conversion

		/* Check if data is available in the FIFO.
		 * If FIFO is empty exit the loop, wait until atleast 1 byte is available */
		// TODO : Check if this returns one even after a byte is cleared from the FIFO
		while(((CT_UART.LSR & 0x1) == 0x0) && (time_cnt > 0)) {
			time_cnt -= 1;
		}

		if(CT_UART.LSR & 0x1) {
			/* Read the value from RBR */
			buffer[cnt] = CT_UART.RBR;

		} else if(time_cnt == 0) {
			/* timeout occurred */
			break;
		}
			

	}

	return cnt;
}


/************************************************
 * UARTClose
 * Remove power to the UART module
 ************************************************/ 
void UARTClose() {
	/* Disable UART before halting */
	CT_UART.PWREMU_MGMT = 0x0;
}

