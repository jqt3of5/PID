#include <math.h>
#include "pid.h"

float Kp = 4;
float Ki = 0.1;
float Kd = 10;

float Td = 20;
float Ti = 20;

float _sp = 0;

float _pv;
float _time;
float integratedError = 0;
float derivedError = 0;

void tune(float kp, float ki, float kd)
{
	Kp = kp;
	Ki = ki;
	Kd = kd;
}

void setPoint(float sp)
{
	_sp = sp;
}

void setProcessVariable(float pv, float time)
{
	auto timeStep = time - _time;
	integratedError += (_sp - pv) * timeStep;

	if (timeStep != 0)
	{
		//derivedError = ((_sp - pv) - (_sp - _pv)) / timeStep;
		derivedError = (_pv - pv) / timeStep;
	}

	_time = time;
	_pv = pv;
}

float error()
{
	return _sp - _pv;
}

float getManipulatedVariable()
{
	auto p = Kp * error();
	auto i = Ki/Ti * integratedError;
	auto d = Kd * Td* derivedError;
	
	return p + i + d;
}

void setIntegralWindupMode(WindupMode mode)
{

}
