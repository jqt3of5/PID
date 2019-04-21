#include "pid.h"
#include <math.h>

float Kp = 2;
float Ki = 20;
float Kd = 5;
float _sp = 0;

int _measurements = 0;
int _index;
float _times[100];
float _pv[100];

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
	_times[_index] = time;
	_pv[_index] = pv;

	if (_measurements < 100)
	{
		_measurements += 1;
	}
	
	_index = (_index + 1) % 100;
}

float error()
{
	return _sp - _pv[_index];
}

float integrateError()
{
	//Array is a ring of measurements
	auto start = _measurements == 100 ? _index : 0;

	float error = 0;
	for (int i = 0, j = start; i < _measurements - 1; i++, j = (j + 1) % 100)
	{
		//Invariant: error = integrate(start, j, pv over dt)
		auto timeStep = _times[(j + 1) % 100] - _times[j];
		error += (_sp - _pv[j]) * timeStep;
	}
	return error;
}

float derivativeError()
{
	//Cant get derivative from a single measurement
	if (_measurements == 1)
	{
		return 0;
	}
	auto error1 = _sp - _pv[_index];
	auto error2 = _sp - _pv[(_index - 1 + 100) % 100];

	return error1 - error2;
}

float getManipulatedVariable()
{
	if (_measurements == 0) 
	{
		return 0;
	}

	auto p = Kp * error();
	auto i = Ki * integrateError();
	auto d = Kd * derivativeError();

	return p + i + d;
}

