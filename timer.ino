#include <sam.h>
#include "timer.h"

// TODO(phil): move defines in all files
#define TIMER_DEFAULT_MS 1000

// TODO(phil): double check this against the manual
void timer3_init() {
	// Setup clock
	GCLK->CLKCTRL.bit.CLKEN = 1;
	GCLK->CLKCTRL.bit.GEN = 0;
	GCLK->CLKCTRL.bit.ID = GCLK_CLKCTRL_ID_TCC2_TC3;
  while (GCLK->STATUS.bit.SYNCBUSY);

  timer3_reset();

	// Setup timer/counter
	TC3->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  TC3->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

  timer3_set(TIMER_DEFAULT_MS);

  // Enable the TC3 interrupt request
  TC3->COUNT16.INTENSET.bit.MC0 = 1;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

  NVIC_EnableIRQ(TC3_IRQn);

  // Enable TC3
  TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}

void timer3_reset() {
  // Disable TC3
  TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

  // Reset TC3
  TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
  while (TC3->COUNT16.CTRLA.bit.SWRST);
}

// TODO(phil): move this
#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024

void timer3_set(uint16_t period_ms) {
  uint16_t hertz = 1000 / period_ms;
  // TODO(phil): how does this work?
  uint16_t ccValue = CPU_HZ / (TIMER_PRESCALER_DIV * hertz) - 1;
  TC3->COUNT16.CC[0].reg = ccValue;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}
