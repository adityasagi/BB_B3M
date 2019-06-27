#include "b3m.h"
#include "gpio.h"


void verify_response(uint8_t *pkt, uint8_t *resp, uint8_t len) {

	uint8_t resp_len;
	uint8_t i, sum;
	resp_len = resp[0];

	// Verify the length of the received data
	if(resp_len != len) {
		debug_lights(20);
		while(1){};
	}

	// Verify the response command
	if(resp[1] != (pkt[1] | 0x80)) {
		debug_lights(5);
		while(1){};
	}

	// Calculate the checksum
	sum=0;
	for(i=0; i< len-1; i++){
		sum += resp[i];
	}

	// Verify the checksum
	if(sum != resp[len-1]){
		debug_lights(100);
		while(1){};
	}

	// If all is well, return
	return;
}
