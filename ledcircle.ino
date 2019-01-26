#include <sam.h>
#include "ledcircle.h"

/*
 * ledcircle_select - Turn on the selected LED. Turn off all other LEDs.
 * @led - Charlieplexed LEDs D1-16
 */
void ledcircle_select(uint8_t led) {

  /*
   * This almost gurantees that all other LEDs are turned off. Just using high
   * impedence (DIRCLR) does not seem to work. The case we ran into was turning
   * on LED 1, 4, and 7 at the same time allow LED 2 to be dimly lit. We think
   * this is because D7 sets IO6 to high and D4 sets IO5 to low such that the
   * high impedence was not set fast enough on both wires for D2 dinode to be
   * ignited.  By triggering all GPIO pins from high impedence, output drive
   * high, output drive low, and back to high impedence, this is harder for the
   * case we had to occur.
   */
  PORT->Group[0].DIRCLR.reg = IO5 | IO6 | IO7 | IO8 | IO9;
  PORT->Group[0].DIRSET.reg = IO5 | IO6 | IO7 | IO8 | IO9;
  PORT->Group[0].OUTSET.reg = IO5 | IO6 | IO7 | IO8 | IO9;
  PORT->Group[0].OUTCLR.reg = IO5 | IO6 | IO7 | IO8 | IO9;
  PORT->Group[0].DIRCLR.reg = IO5 | IO6 | IO7 | IO8 | IO9;

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
