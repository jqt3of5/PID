
#include "MAX31855.h"
#include <Wire.h>

static int _ss_pin = 0;

static const double ProbeStep = .25; // C/bit
static const double ColdStep = .0625; // C/bit


void MAX31855_setup(int ssPin){
  _ss_pin = ssPin;
  pinMode(_ss_pin, OUTPUT);
  digitalWrite(_ss_pin, HIGH);
  SPI1.setDataMode(SPI_MODE0);
}

bool MAX31855_read(double * probeTemp, double * coldTemp, char * status) {
  //D5 SS
  SPI1.begin();
  //Select chip
  digitalWrite(_ss_pin, LOW);

  char rx[4] = {0};
  SPI1.transfer(NULL, rx, sizeof(rx), NULL);

  //deselect chip
  digitalWrite(_ss_pin, HIGH);
  SPI1.end();

  if (SPI1.available() > 0)
  {
    *status = rx[3] & 0xF;

    //Doesnt' handle negative temps...
    *probeTemp = ProbeStep * ((rx[0] << 6) | (rx[1] >> 2));
    *coldTemp = ColdStep * ((rx[2] << 4) | (rx[3] >> 4));
    return true;
  }
  return false;
}
