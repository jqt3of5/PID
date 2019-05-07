#pragma once
typedef int WindupMode;

extern double pid_setPoint;
extern double pid_manipulatedVariable;


void pid_setProcessVariable(double pv, double time);
double pid_updateManipulatedVariable();

const int EnableWhenControllable = 1, CapIntegralTerm = 2, EraseHistory = 3;
void setIntegralWindupMode(WindupMode mode);

//Loop cycletime
//deadtime
