#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <pru_cfg.h>




void initGPIO();
void toggleGPIO(uint32_t gpio);
void setGPIO(uint32_t gpio);
void clearGPIO(uint32_t gpio);
void debug_lights(uint8_t cnt);

#endif // GPIO_H
