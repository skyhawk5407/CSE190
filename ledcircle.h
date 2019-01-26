#ifndef LEDCIRCLE_H
#define LEDCIRCLE_H

/*
 * ledcircle_select - Turn on the selected LED. Turn off all other LEDs.
 * @led - Charlieplexed LEDs D1-16
 */
void ledcircle_select(uint8_t led);

#define IO5 PORT_PA15
#define IO6 PORT_PA20
#define IO7 PORT_PA21
#define IO8 PORT_PA06
#define IO9 PORT_PA07

/*
 * Set these port as output
 * First argument is drive the port high
 * Second argument is drive the port low
 */
#define HIGH_LOW(a,b) \
      PORT->Group[0].DIRSET.reg = a | b; \
      PORT->Group[0].OUTSET.reg = a;     \
      PORT->Group[0].OUTCLR.reg = b;     \

#endif
