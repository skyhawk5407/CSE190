#include <sam.h>

#include "ledcircle.h"
#include "timer.h"

#define PIN_IO5 31
#define PIN_IO6 29
#define PIN_IO7 27
#define PIN_IO8 25
#define PIN_IO9 23

#define GET_BIT(x,i) (((x) & (1 << i)) >> i)
#define NUM_LEDS 16
#define NUM_BITS_IN_BYTE 8

/* === DO NOT REMOVE: Initialize C library === */
extern "C" void __libc_init_array(void);

static char *name = "phil";
static uint8_t nameLength = 4;
static uint8_t nameIndex = 0;

enum State { CHAR_STATE, RESET_STATE };

static uint8_t state = 0;

static uint8_t volatile changeLEDs;

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
* TODO(phil): fixme
 * turnOnLEDs - set leds by bits given by character in name array.
 *
 * @onLEDs: array that corresponds to the state of an LED light (0=off, 1=on)
 * @offset: what led should the second character start at since we are showing
 * two bytes at a time
 */
void turnOnLEDs(uint8_t *onLEDs, uint8_t offset, char *name, uint8_t index) {
  char c = name[index];

  for (int b = 0; b < NUM_BITS_IN_BYTE; b++) {
    if (GET_BIT(c,b)) {
      onLEDs[b+offset] = 1;
    }
  }
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
  //timer3_set(TIMER_PERIOD_MS);

  uint8_t onLEDs[NUM_LEDS];
  clearLEDs(onLEDs);

  State state = RESET_STATE;

  /* === Main Loop === */
  while (1) {
    /* Lower part of TC3 interrupt */
    if (changeLEDs) {
      changeLEDs = 0;
      ledcircle_select(0);
      clearLEDs(onLEDs);

      switch (state) {
        case CHAR_STATE:
        {
          turnOnLEDs(onLEDs, 0, name, nameIndex);
          nameIndex++;

          if (nameIndex >= nameLength) { // odd check
            state = RESET_STATE;
          } else {
            turnOnLEDs(onLEDs, NUM_BITS_IN_BYTE, name, nameIndex);
            nameIndex++;
          }

          if (nameIndex >= nameLength) { // even check
            state = RESET_STATE;
          }
        } break;

        case RESET_STATE:
        {
          nameIndex = 0;
          state = CHAR_STATE;
          ledcircle_select(0);
        } break;
      }
    }

#if 1
    for (int i = 1; i <= 16; i++) {
      if (onLEDs[i-1]) {
        ledcircle_select(i);
      }
    }
#else
        ledcircle_select(1);
        ledcircle_select(4);
        ledcircle_select(7);
#endif
  }

  return 0;
}
