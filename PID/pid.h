#pragma once
typedef int WindupMode;

void setPoint(float sp);
void setProcessVariable(float pv, float time);
float getManipulatedVariable();

const int EnableWhenControllable = 1, CapIntegralTerm = 2, EraseHistory = 3; 
void setIntegralWindupMode(WindupMode mode);

//Loop cycletime
//deadtime