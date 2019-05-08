#pragma once

extern double pid_setPoint;
extern double pid_manipulatedVariable;

typedef int WindupMode;
const int Normal = 0, EnableWhenControllable = 1, CapIntegralTerm = 2, EraseHistory = 3;

void pid_init( double maxOutput = 255, double minOutput = 0, WindupMode mode = Normal);
void pid_tune(double kp, double ki, double kd);
double pid_update(double withProcessVariable, double atTime);

//Loop cycletime
//deadtime
