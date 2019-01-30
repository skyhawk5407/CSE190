#ifndef TIMER_h
#define TIMER_h

#define TIMER_DEFAULT_MS 1000
#define CPU_HZ 48000000
#define PRESCALER_DIV 1024
#define ONE_mHZ 1000
#define TIMER_PERIOD_MS 50

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

/*
 * timer3_set - Set the period that the timer will fire (in milliseconds). A
 * timer interrupt (TC3_Handler) will be fired for each timer period.
 *
 * @period_ms - length of the period in milliseconds where the interrupt will be called
 *
 * Example 1: period_ms = 1000 ms (1 second)
 * First, we convert milliseconds to hertz.
 * 1000 mHz / 1000 ms = 1 Hz
 * Then to get the ccValue we take the CPU hertz since we are using generator 0
 * and divide it by the prescaler times the period hertz. We minus 1 because we
 * want it to reset right when we match CC0.
 * ccValue = (48,000,000)/(1,024 * 1) = 46,875
 * To get how many seconds the interrupt will be fired, we multiply the
 * 1/(oscillator frequency) which is 1/(cpu hz prescaled).
 * 1/(48,000,000/1,024) * (46,875) = 1 second
 *
 * Example 2: period_ms = 50 ms
 * 1000 mHz / 50 ms = 20 Hz
 * ccValue = (48,000,000)/(1,024 * 20) = 2343
 * 1/(48,000,000/1,024) * 2343 = 0.05 second = 50 millisecond
 */
void timer3_set(uint16_t period_ms);

#endif
