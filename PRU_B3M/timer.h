#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <pru_iep.h>


void initTimer(uint32_t timeout);
void startTimer();
void waitForTimeout();
void stopTimer();

#endif // TIMER_H
