#include <sam.h>

#include "ledcircle.h"
#include "timer.h"

/* === DO NOT REMOVE: Initialize C library === */
extern "C" void __libc_init_array(void);

static char *name = "phil";
static uint8_t nameLength = 4;

static uint8_t volatile enableLEDs;
static uint8_t volatile nameIndex;

// TODO(phil): move defines in all c files to header files
#define GET_LSB_NIBBLE(x) ((x) & 0xF)
#define GET_MSB_NIBBLE(x) (((x) & 0xF) >> 4)

#define GET_BIT(x,i) (((x) & (1 << i)) >> i)

void TC3_Handler(void) {
  enableLEDs = 1;

  TC3->COUNT16.INTFLAG.bit.MC0 = 1;
}

void clearLEDs(uint8_t *onLEDs) {
  for (int i = 1; i <= 16; i++) {
    onLEDs[i] = 0;
  }
}

// return value is reset occurs
uint8_t turnOnLEDs(uint8_t *onLEDs, uint8_t offset) {
  uint8_t reset;

  nameIndex++;

  if (nameIndex == nameLength) {
    ledcircle_select(0);
    nameIndex = 0;
    reset = 1;
  } else {
    char c = name[nameIndex];

    for (int b = 0; b < 8; b++) {
      if (GET_BIT(c,b)) {
        onLEDs[b+offset] = 1;
      }
    }
    reset = 0;
  }

  return reset;
}

// TODO(phil): add header comments to all functions
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
  // TODO(phil): make this a define
  timer3_set(50); // 20 hz

  uint8_t onLEDs[16];
  clearLEDs(onLEDs);

  /* === Main Loop === */
  while (1) {
    // TODO Your implementation here
    if (enableLEDs) {
      clearLEDs(onLEDs);

      uint8_t reset = turnOnLEDs(onLEDs, 0);
      if (!reset) {
        turnOnLEDs(onLEDs, 8);
      }

      enableLEDs = 0;
    }

    for (int i = 1; i <= 16; i++) {
      if (onLEDs[i-1]) {
        ledcircle_select(i);
      }
    }
  }

  return 0;
}
