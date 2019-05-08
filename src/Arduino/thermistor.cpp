#include <math.h>
#include <Wire.h>
#include "thermistor.h"

//const double MAX_A = 4095.0;
//const double R1 = 100000.0;

const double A = 0.0008675874131;
const double B = 0.0001992667233;
const double C = 0.0000001323120683;

double thermistor_analogToTemp(int analog, bool inCelcius, int analogBits, int resistor)
{
  double maxAnalog = powf(2, analogBits) - 1;

  double avg_R0 = analog*resistor/(maxAnalog - analog);
  double temp = 1/(A + B*log(avg_R0) + C*log(avg_R0)*log(avg_R0)*log(avg_R0)) - 273.15;

  return inCelcius ? temp : toF(temp);
}

double toC(double f)
{
    return (f - 32.0) * 5.0/9.0;
}

double toF(double c)
{
    return c * (9.0/5.0) + 32.0;
}
