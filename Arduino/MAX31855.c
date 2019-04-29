#include <Wire.h>
#include "MAX31855.h"

const int tCSS = 100; //ns
const int tCH = 100;
const int tCL = 100;
const int tDO = 40;
const int tDV = 100;
const int tTR = 40;

int _do = -1, _clk = -1, _cs = -1;

void max31855_setup_pins(int dataOut, int clk, int cs);
{
    _do = dataOut;
    _clk = clk;
    _cs = cs;

    pinMode(_do, INPUT_PULLUP);

    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    pinMode(_clk, OUTPUT);
    digitalWrite(_clk, LOW);

}

void delay_ns(int ns)
{
  for (int i = 0; i < ns/10; ++i)
  {
    nop();
  }
}

void read_temp(int & probeTemp, int & coldTemp);
{
  noInterrupts();

  pinResetFast(_cs);
  delay_ns(_tDV);

//TODO: Improve the speed of this read - Currently it's pretty slow, as the
//number clock cycles is not optimized
  probeTemp = 0;
  for (int i = 15; i >= 0; --i)
  {
    probeTemp |= pinReadFast(_do) << i;

    pinSetFast(_clk);
    delay_ns(_tCH);
    pinResetFast(_clk);
    delay_ns(_tCL);
  }

  coldTemp = 0;
  for (int i = 15; i >= 0; --i)
  {
    coldTemp |= pinReadFast(_do) << i;

    pinSetFast(_clk);
    delay_ns(_tCH);
    pinResetFast(_clk);
    delay_ns(_tCL);
  }

  pinSetFast(_cs);

  interrupts();
}
