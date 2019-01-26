#ifndef TIMER_h
#define TIMER_h

#define TIMER_DEFAULT_MS 1000
#define CPU_HZ 48000000
#define PRESCALER_DIV 1024
#define ONE_mHZ 1000

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
void timer3_init();

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
void timer3_reset();

// TODO(phil): add comment header
void timer3_set(uint16_t period_ms);

#endif
