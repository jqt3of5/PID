#include "MAX31855.h"
#include "pid.h"
#include <Wire.h>
#include <string.h>

enum SensorType {ThermocoupleSPI, ThermistorAnalog};

struct Config {
  int coolPin, heatPin;
  SensorType type;
  int thermistorPin;
} _config = {D0, D1, ThermocoupleSPI};

double _probeTemp = 0;
double _coldTemp = 0;
double _currentTime = 0;

char _status = 0;
bool _idle = true;

int setTargetTemp(String extra)
{
  _idle = false;
  //Pro Tip: %fand %lf aren't actually supported
  //sscanf(extra.c_str(), "%lf", &pid_setPoint);

  pid_setPoint = atof(extra.c_str());
  Serial.printf("%lf\n", pid_setPoint);

  return 0;
}

void setup() {
  Serial.begin(9600);

  Particle.variable("time", _currentTime);

  Particle.variable("manipulatedVariable", pid_manipulatedVariable);
  Particle.variable("setPoint", pid_setPoint);

  Particle.variable("coldTemp", _coldTemp);
  Particle.variable("probeTemp", _probeTemp);
  Particle.variable("status", _status);

  Particle.function("setTargetTemp", setTargetTemp);

  if (_config.type == ThermocoupleSPI)
  {
    MAX31855_setup();
  }

}

void loop() {

    if (MAX31855_read(&_probeTemp, &_coldTemp, &_status))
    {
      _currentTime = millis()/1000.0f;
      if (!_idle) {
        pid_setProcessVariable(_probeTemp, _currentTime);
      }

      Serial.printf("%f %f",_probeTemp, _coldTemp);
      Serial.println();
    }
}
