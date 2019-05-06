#include "MAX31855.h"
#include <Wire.h>
double _probeTemp = 0;
double _coldTemp = 0;
char _status = 0;


void setup() {
//  max31855_setup_pins(D0,D1,D2);
  Serial.begin(9600);

  Particle.variable("coldTemp", _coldTemp);
  Particle.variable("probeTemp", _probeTemp);
  Particle.variable("status", _status);

  MAX31855_setup();
}

void loop() {

    if (MAX31855_read(&_probeTemp, &_coldTemp, &_status))
    {

      Serial.printf("%f %f",_probeTemp,_coldTemp);
      Serial.println();
    }
}
