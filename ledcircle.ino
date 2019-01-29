#include <sam.h>
#include "ledcircle.h"

#define PIN_IO5 31
#define PIN_IO6 29
#define PIN_IO7 27
#define PIN_IO8 25
#define PIN_IO9 23

/*
 * ledcircle_select - Turn on the selected LED. Turn off all other LEDs.
 * @led - Charlieplexed LEDs D1-16
 */
void ledcircle_select(uint8_t led) {

  /*
   * This almost guarantees that all other LEDs are turned off. Just using high
   * impedance (DIRCLR) does not seem to work. The case we ran into was turning
   * on LED 1, 4, and 7 at the same time which allows LED 2 to be dimly lit. We
   * think this is because D7 drives IO6 high and D4 drives IO5 low such that
   * the high impedance was not set fast enough on both wires for D2 diode to
   * be ignited.  By setting all GPIO pins to high impedance, then output
   * drives high, output drive low, and back to high impedance, this is made it
   * harder for the case we had to occur.
   */
  PORT->Group[0].DIRCLR.reg = IO5 | IO6 | IO7 | IO8 | IO9;

  switch (led)
  {
    case 1:
      {
        HIGH_LOW(IO5, IO6);
        PORT->Group[0].DIRCLR.reg = IO7 | IO8 | IO9;
      } break;

    case 2:
      {
        HIGH_LOW(IO6, IO5);
        PORT->Group[0].DIRCLR.reg = IO7 | IO8 | IO9;
      } break;

    case 3:
      {
        HIGH_LOW(IO5, IO7);
        PORT->Group[0].DIRCLR.reg = IO6 | IO8 | IO9;
      } break;

    case 4:
      {
        HIGH_LOW(IO7, IO5);
        PORT->Group[0].DIRCLR.reg = IO6 | IO8 | IO9;
      } break;

    case 5:
      {
        HIGH_LOW(IO6, IO7);
        PORT->Group[0].DIRCLR.reg = IO5 | IO8 | IO9;
      } break;

    case 6:
      {
        HIGH_LOW(IO7, IO6);
        PORT->Group[0].DIRCLR.reg = IO5 | IO8 | IO9;
      } break;

    case 7:
      {
        HIGH_LOW(IO6, IO8);
        PORT->Group[0].DIRCLR.reg = IO7 | IO5 | IO9;
      } break;

    case 8:
      {
        HIGH_LOW(IO8, IO6);
        PORT->Group[0].DIRCLR.reg = IO7 | IO5 | IO9;
      } break;

    case 9:
      {
        HIGH_LOW(IO5, IO8);
        PORT->Group[0].DIRCLR.reg = IO6 | IO7 | IO9;
      } break;

    case 10:
      {
        HIGH_LOW(IO8, IO5);
        PORT->Group[0].DIRCLR.reg = IO6 | IO7 | IO9;
      } break;

    case 11:
      {
        HIGH_LOW(IO8, IO7);
        PORT->Group[0].DIRCLR.reg = IO6 | IO5 | IO9;
      } break;

    case 12:
      {
        HIGH_LOW(IO7, IO8);
        PORT->Group[0].DIRCLR.reg = IO6 | IO5 | IO9;
      } break;

    case 13:
      {
        HIGH_LOW(IO9, IO7);
        PORT->Group[0].DIRCLR.reg = IO6 | IO5 | IO8;
      } break;

    case 14:
      {
        HIGH_LOW(IO7, IO9);
        PORT->Group[0].DIRCLR.reg = IO6 | IO5 | IO8;
      } break;

    case 15:
      {
        HIGH_LOW(IO9, IO8);
        PORT->Group[0].DIRCLR.reg = IO6 | IO5 | IO7;
      } break;

    case 16:
      {
        HIGH_LOW(IO8, IO9);
        PORT->Group[0].DIRCLR.reg = IO5 | IO6 | IO7;
      } break;

    default:
      {
      } break;
  }

  // Bright lights
  for (int i = 0; i < 32; i++) {
    PORT->Group[0].PINCFG[PIN_IO5].bit.DRVSTR = i;
  }
}
