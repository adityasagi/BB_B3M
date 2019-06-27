#include "timer.h"



void wait(uint32_t cnt) {
	uint32_t i;
	volatile uint32_t count; // To force the compiler to avoid optimization
	count = cnt;
	i=0;
	while(i < count) {
		i++;
	}
}
