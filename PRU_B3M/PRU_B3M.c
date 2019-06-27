/*
 * Modified from example code provided by TI.
 *
 * Copyright (C) 2019 RBCCPS, IISc
 * 
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "uart.h"
#include "timer.h"
#include "b3m.h"
#include "resource_table_0.h"

#define TRUE 1
#define FALSE 0


void main(void) {

	uint8_t buffer[25];
	uint8_t recv_len;
	uint8_t expect_reply;

	// Set to FREE MODE
	uint8_t pkt1[]={0x08, 0x04, 0x00, 0x00, 0x02, 0x28, 0x01, 0x37};

	// Set to VELOCITY CONTROL MODE
	uint8_t pkt2[]={0x08, 0x04, 0x00, 0x00, 0x06, 0x28, 0x01, 0x3B};

	// Set the GAIN PRESETS
	uint8_t pkt3[]={0x08, 0x04, 0x00, 0x00, 0x01, 0x5c, 0x01, 0x6A};

	// Set to NORMAL MODE
	uint8_t pkt4[]={0x08, 0x04, 0x00, 0x00, 0x04, 0x28, 0x01, 0x39};

	// Set the VELOCITY
	uint8_t pkt5[]={0x09, 0x04, 0x00, 0x00, 0x10, 0x27, 0x30, 0x01, 0x75};

	// Set the VELOCITY (set to 0 velocity)
	uint8_t pkt6[]={0x09, 0x04, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x3E};



	/* Initialisation */
	UARTInit();


	UARTSend(pkt1, pkt1[0]); // FREE MODE
	expect_reply = TRUE;
	if(expect_reply){
		recv_len = 0x05;
		UARTReceive(buffer, recv_len, 3);
		verify_response(pkt1,  buffer, recv_len);
	}

	wait(5000);
	UARTSend(pkt2, pkt2[0]); // VELOCITY CONTROL
	expect_reply = TRUE;
	if(expect_reply){
		recv_len = 0x05;
		UARTReceive(buffer, recv_len, 3);
		verify_response(pkt2, buffer, recv_len);
	}

	wait(5000);
	UARTSend(pkt3, pkt3[0]); // GAIN PRESET 1
	expect_reply = TRUE;
	if(expect_reply){
		recv_len = 0x05;
		UARTReceive(buffer, recv_len, 3);
		verify_response(pkt3, buffer, recv_len);

	}

	wait(5000);
	UARTSend(pkt4, pkt4[0]); // NORMAL MODE
	expect_reply = TRUE;
	if(expect_reply){
		recv_len = 0x05;
		UARTReceive(buffer, recv_len, 3);
		verify_response(pkt4, buffer, recv_len);
	}

	while(1) { // Toggle between RUNNING and STOP
		wait(50000000);
		UARTSend(pkt5, pkt5[0]); // RUN
		expect_reply = TRUE;
		if(expect_reply) {
			recv_len = 0x05;
			UARTReceive(buffer, recv_len, 3);
			verify_response(pkt5, buffer, recv_len);

		}

		wait(50000000);	
		UARTSend(pkt6, pkt6[0]); // STOP (0 velocity)
		expect_reply = TRUE;
		if(expect_reply) {
			recv_len = 0x05;
			UARTReceive(buffer, recv_len, 3);
			verify_response(pkt6, buffer, recv_len);
		}

	}

	UARTClose();



	/* Halt PRU core */
	__halt();
}
