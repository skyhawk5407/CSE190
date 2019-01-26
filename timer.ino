#include <sam.h>
#include "timer.h"

/*
 * timer3_init - Setup timer 3. Stop the timer, clear out any timer state and
 * reset all counters. Set the clock the timer uses and enable interrupts.
 *
 * Initializes GCLK's clock control to use Generic clock generator 0 and TC3.
 * TC3 is using 16-bit counter mode with a prescaler of 1024. The waveform
 * configuration is set to Match frequency (MFRQ). This means the counter will
 * reset back to 0 at CC0. The counter period is changed by writing a new TOP
 * value to the Period register (PER or CC0, depending on the waveform
 * generation mode). The counter value is continuously compared with each CC0
 * value. An interrupt will be generated on comparison match when
 * INTENSET.MC0=1.
 */
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

/*
 * timer3_reset - Reset timer 3’s counters, but does not reset the peripheral.
 * The timer can be in the middle of execution when it is reset.
 *
 * Trigger a software reset which resets all registers in the TC, except
 * DBGCTRL, to their initial state, and the TC will be disabled. Due to
 * synchronization there is a delay from writing CTRLA.SWRST until the reset is
 * complete. CTRLA.SWRST and SYNCBUSY.SWRST will both be cleared when the reset
 * is complete.
 */
void timer3_reset() {
  // Disable TC3
  TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

  // Reset TC3
  TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
  while (TC3->COUNT16.CTRLA.bit.SWRST);
}

/*
 * timer3_set - Set the period that the timer will fire (in milliseconds). A
 * timer interrupt (TC3_Handler) will be fired for each timer period.
 *
 * @period_ms - length of the period in milliseconds where the interrupt will be called
 *
 * TODO(phil): double check this
 * Example 1: period_ms = 1000 ms (1 second)
 * First, we convert milliseconds to hertz.
 * 1000 mHz / 1000 ms = 1 Hz
 * Then to get the ccValue we take the CPU hertz since we are using generator 0
 * and divide it by the prescaler times the period hertz. We minus 1 because we
 * want it to reset right when we match CC0.
 * ccValue = (48,000,000)/(1,024 * 1) = 46,875
 * To get how many (FIXME: ???) the interrupt will be fired we multiply the
 * 1/ccValue by (oscillator frequency) which is (cpu hz prescaled).
 * (48,000,000/1,024) * (1/46,875) = 1
 *
 * Example 2: period_ms = 50 ms
 * 1000 mHz / 50 ms = 20 Hz
 * ccValue = (48,000,000)/(1,024 * 20) = 2343
 * (48,000,000/1,024) * (1/46,875) = 20 (FIXME clock cycles?)
 */
void timer3_set(uint16_t period_ms) {
  uint16_t hertz = ONE_mHZ / period_ms;
  uint16_t ccValue = CPU_HZ / (PRESCALER_DIV * hertz) - 1;
  TC3->COUNT16.CC[0].reg = ccValue;
  while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}
