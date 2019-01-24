#include <sam.h>
#include "ledcircle.h"

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
      PORT->Group[0].OUT.reg = a; \
      PORT->Group[0].OUTCLR.reg = b; \

void ledcircle_select(uint8_t led) {
  // set to high impedence
  PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA20 | PORT_PA21 | PORT_PA11 | PORT_PA12;

  switch (led)
  {
    case 1:
    {
      HIGH_LOW(IO5, IO6);
    } break;

    case 2:
    {
      HIGH_LOW(IO6, IO5);
    } break;

    case 3:
    {
      HIGH_LOW(IO5, IO7);
    } break;

    case 4:
    {
      HIGH_LOW(IO7, IO5);
    } break;

    case 5:
    {
      HIGH_LOW(IO6, IO7);
    } break;

    case 6:
    {
      HIGH_LOW(IO7, IO6);
    } break;

    case 7:
    {
      HIGH_LOW(IO6, IO8);
    } break;

    case 8:
    {
      HIGH_LOW(IO8, IO6);
    } break;

    case 9:
    {
      HIGH_LOW(IO5, IO8);
    } break;

    case 10:
    {
      HIGH_LOW(IO8, IO5);
    } break;

    case 11:
    {
      HIGH_LOW(IO8, IO7);
    } break;

    case 12:
    {
      HIGH_LOW(IO7, IO8);
    } break;

    case 13:
    {
      HIGH_LOW(IO9, IO7);
    } break;

    case 14:
    {
      HIGH_LOW(IO7, IO9);
    } break;

    case 15:
    {
      HIGH_LOW(IO9, IO8);
    } break;

    case 16:
    {
      HIGH_LOW(IO8, IO9);
    } break;

    default:
    {
    } break;
  }
}
