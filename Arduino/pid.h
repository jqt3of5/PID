#pragma once
typedef int WindupMode;

void setPoint(double sp);
void setProcessVariable(double pv, double time);
double getManipulatedVariable();

const int EnableWhenControllable = 1, CapIntegralTerm = 2, EraseHistory = 3;
void setIntegralWindupMode(WindupMode mode);

//Loop cycletime
//deadtime
