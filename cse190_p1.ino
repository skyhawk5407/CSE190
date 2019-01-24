#include <sam.h>

#include "ledcircle.h"
#include "timer.h"

/* === DO NOT REMOVE: Initialize C library === */
extern "C" void __libc_init_array(void);

static volatile int turnOnLed;
static volatile uint8_t led;

// TODO(phil): move defines in all c files to header files

void TC3_Handler(void) {
  turnOnLed = 1;
  if (led == 16) led = 0;
  led++;
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; //don't change this, it's part of the timer code
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

  /* === Main Loop === */
  while (1) {
    // TODO Your implementation here
    if (turnOnLed) {
      ledcircle_select(led);
      turnOnLed = 0;
    }

// TODO(phil): remove this?
#if 0
    for (int i = 1; i <= 16; i++) {
      ledcircle_select(i);
    }
#endif
  }

  return 0;
}
