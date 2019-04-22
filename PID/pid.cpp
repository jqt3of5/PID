#include "pid.h"
#include <math.h>

float Kp = 2;
float Ki = .00025;
float Kd = 35;
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
	derivedError = ((_sp - pv) - (_sp - _pv))/ timeStep;

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
	auto i = Ki * integratedError;
	auto d = Kd * derivedError;
	
	return p + i + d;
}

