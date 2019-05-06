#include "MAX31855.h"
#include "pid.h"
#include <Wire.h>
#include <string.h>

double _probeTemp = 0;
double _coldTemp = 0;
double _currentTime = 0;
double _manipulatedVariable = 0;
double _setPoint = 0;

char _status = 0;

bool _idle = true;

int setTargetTemp(String extra)
{
  _idle = false;


  sscanf(extra.c_str(), "%lf", &_setPoint);
  Serial.printf("%lf\n", _setPoint);
  Serial.printf("%s\n", extra.c_str());
  setPoint(_setPoint);

  return 0;
}

void setup() {
  Serial.begin(9600);

  Particle.variable("time", _currentTime);
  Particle.variable("coldTemp", _coldTemp);
  Particle.variable("probeTemp", _probeTemp);
  Particle.variable("manipulatedVariable", _manipulatedVariable);
  Particle.variable("setPoint", _setPoint);
  Particle.variable("status", _status);

  Particle.function("setTargetTemp", setTargetTemp);
  MAX31855_setup();
}

void loop() {

    if (MAX31855_read(&_probeTemp, &_coldTemp, &_status))
    {
      _currentTime = millis()/1000.0f;
      if (!_idle) {
        setProcessVariable(_probeTemp, _currentTime);
        _manipulatedVariable = getManipulatedVariable();
      }

      //Serial.printf("%f %f",_probeTemp, _coldTemp);
      //Serial.println();
    }
}
