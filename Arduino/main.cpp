#include "MAX31855.h"
#include "pid.h"
#include <Wire.h>
#include <string.h>

enum SensorType {ThermocoupleSPI, ThermistorAnalog};

struct Config {
  int coolPin, heatPin;
  int sampleTimeSeconds;
  SensorType type;
  int thermistorPin;
} _config = {D0, D1, 10, ThermocoupleSPI};

double _probeTemp = 0;
double _ambientTemp = 0;
double _currentTime = 0;

char _status = 0;

bool _idle = true;
int setTargetTemp(String extra)
{
  _idle = false;
  //Pro Tip: %fand %lf aren't actually supported by scanf!
  //sscanf(extra.c_str(), "%lf", &pid_setPoint);

  pid_setPoint = atof(extra.c_str());
  Serial.printf("%lf\n", pid_setPoint);

  return 0;
}

void setup() {
  Serial.begin(9600);

  Particle.variable("manipulatedVariable", pid_manipulatedVariable);
  Particle.variable("setPoint", pid_setPoint);

  Particle.variable("ambientTemp", _ambientTemp);
  Particle.variable("probeTemp", _probeTemp);
  Particle.variable("status", _status);

  Particle.function("setTargetTemp", setTargetTemp);

  if (_config.type == ThermocoupleSPI)
  {
    MAX31855_setup();
  }

  pid_init();

  pinMode(_config.heatPin, OUTPUT);
  pinMode(_config.coolPin, OUTPUT);
}

bool updateReadingsIfNeeded()
{
  auto time =  millis()/1000.0f;
  auto timeDiff = time - _currentTime;
  if (timeDiff > _config.sampleTimeSeconds)
  {
    _currentTime = time;
    if (MAX31855_read(&_probeTemp, &_ambientTemp, &_status))
    {
      if (!_idle) {
          pid_update(_probeTemp, _currentTime);
      }

      return true;
    }
  }
  return false;
}

int count = 0;
void loop() {

  if (updateReadingsIfNeeded())
  {
    Serial.printf("%f %f",_probeTemp, _ambientTemp);
    Serial.println();

    char data[100] = {0};
    sprintf(data, "{probe:%lf,ambient:%lf, time:%lf, mv:%lf, sp:%lf}", _probeTemp, _ambientTemp, _currentTime, pid_manipulatedVariable, pid_setPoint);
    Particle.publish("Reading", data, PUBLIC);
  }

  double mv = fabsf(pid_manipulatedVariable);
  //if the abs(mv) is still greater than our count, then we are in the active/high
  //portion of our duty cycle
  bool active = mv >= count ? HIGH : LOW;
  if (pid_manipulatedVariable > 0) //Heating
  {
    digitalWrite(_config.heatPin, active);
    digitalWrite(_config.coolPin, LOW);
  }
  else //cooling
  {
    digitalWrite(_config.coolPin, active);
    digitalWrite(_config.heatPin, LOW);
  }

  count = (count + 1) % 255;
}
