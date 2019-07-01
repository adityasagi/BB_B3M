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
#include "rpmsg.h"
#include "gpio.h"

#define TRUE 1
#define FALSE 0


uint8_t payload[RPMSG_BUF_SIZE];


void main(void) {

	uint8_t buffer[RPMSG_BUF_SIZE];
	uint8_t recv_len;
	uint8_t expect_reply;

	struct pru_rpmsg_transport transport;
	uint16_t src, dst, len;

	/* Initialisation */
	UARTInit();

	RPMsgInit(&transport);

	initTimer(0x1E8480); // 10ms @ 200 MHz

	startTimer();

	/* Task loop */
	while(1) {
		if(RPMsgRecv(&transport, &src, &dst, payload, &len)==0) {

			waitForTimeout();
			UARTSend(payload, payload[0]); 
			expect_reply = TRUE;
			if(expect_reply){
				recv_len=UARTReceive(buffer, 0xFF, 3);
				verify_response(payload,  buffer, recv_len);
				if(recv_len > 0)
					RPMsgSend(&transport, dst, src, buffer, recv_len);
				//debug_lights(recv_len);
			}
		}
	}



	/* Halt PRU core */
	__halt();


}

