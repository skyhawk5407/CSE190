#include <sam.h>

#include "ledcircle.h"
#include "timer.h"

/* === DO NOT REMOVE: Initialize C library === */
extern "C" void __libc_init_array(void);

static char *name = "phil";
static uint8_t nameLength = 4;
static uint8_t nameIndex = 0;

static uint8_t volatile changeLEDs;

#define GET_BIT(x,i) (((x) & (1 << i)) >> i)
#define NUM_LEDS 16
#define TIMER_PERIOD_MS 50
#define NUM_BITS_IN_BYTE 8

/* Upper part of TC3 interrupt */
void TC3_Handler(void) {
  changeLEDs = 1;

  TC3->COUNT16.INTFLAG.bit.MC0 = 1;
}

/*
 * clearLEDs - Set array onLEDs to zero.
 *
 * @onLEDs: array that corresponds to the state of an LED light (0=off, 1=on)
 */
void clearLEDs(uint8_t *onLEDs) {
  for (int i = 0; i < NUM_LEDS; i++) {
    onLEDs[i] = 0;
  }
}

/*
 * turnOnLEDs - set leds by bits given by character in name array.
 *
 * @onLEDs: array that corresponds to the state of an LED light (0=off, 1=on)
 * @offset: what led should the second character start at since we are showing
 * two bytes at a time
 *
 * return: status flag representing when to turn off all leds (reset state)
 */
uint8_t turnOnLEDs(uint8_t *onLEDs, uint8_t offset) {
  uint8_t reset;

  if (nameIndex >= nameLength) {
    nameIndex = 0;
    reset = 1;
  } else {
    char c = name[nameIndex];

    for (int b = 0; b < NUM_BITS_IN_BYTE; b++) {
      if (GET_BIT(c,b)) {
        onLEDs[b+offset] = 1;
      }
    }

    nameIndex++;
    reset = 0;
  }

  return reset;
}

/*
 * main - high data rate (320 bits per second), short-range visual light
 * communication system
 */
int main(void) {
  /* ==== DO NOT REMOVE: USB configuration ==== */
  init();
  __libc_init_array();
  USBDevice.init();
  USBDevice.attach();
  /* =========================================== */

  /* === Init Drivers === */
  ledcircle_select(0);

  timer3_init();
  timer3_set(TIMER_PERIOD_MS);

  uint8_t onLEDs[NUM_LEDS];
  clearLEDs(onLEDs);

  uint8_t reset = 0;

  /* === Main Loop === */
  while (1) {
    /* Lower part of TC3 interrupt */
    if (changeLEDs) {
      clearLEDs(onLEDs);

      // the reset is set on the next called to turnOnLEDs
      if (!reset) {
        reset = turnOnLEDs(onLEDs, 0);
        if (!reset) {
          reset = turnOnLEDs(onLEDs, NUM_BITS_IN_BYTE);
        } else {
          // odd length reset
          ledcircle_select(0);
          reset = 0;
        }
      } else {
        // even length reset
        ledcircle_select(0);
        reset = 0;
      }

      changeLEDs = 0;
    }

    for (int i = 1; i <= 16; i++) {
      if (onLEDs[i-1]) {
        ledcircle_select(i);
      }
    }
  }

  return 0;
}
