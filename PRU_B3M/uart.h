#ifndef UART_H
#define UART_H


#include <stdint.h>
#include <stdio.h>
#include <pru_uart.h>

#define EN_PIN (1 << 5) // P9.27

/* The FIFO size on the PRU UART is 16 bytes */
#define FIFO_SIZE	16
#define MAX_CHARS	8

/* This hostBuffer structure is temporary but stores a data buffer */
//struct {
//	uint8_t msg; // Not used today
//	uint8_t data[FIFO_SIZE];
//} hostBuffer;




void UARTInit();
uint8_t UARTSend(uint8_t*, uint8_t);
uint8_t UARTReceive(uint8_t*, uint8_t, uint32_t timeout);
void UARTClose();

#endif // UART_H

