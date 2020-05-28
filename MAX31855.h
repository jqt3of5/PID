#include <Wire.h>
void MAX31855_setup(int ssPin = D5);
bool MAX31855_read(double * probeTemp, double * coldTemp, char * status);
