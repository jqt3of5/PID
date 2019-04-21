#include "pid.h"
#include <math.h>

#define SAMPLES 1000

float Kp = 10;
float Ki = .1;
float Kd = 4;
float _sp = 0;

int _measurements = 0;
int _index;
float _times[SAMPLES];
float _pv[SAMPLES];

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

	if (_measurements < SAMPLES)
	{
		_measurements += 1;
	}
	
	_index = (_index + 1) % SAMPLES;
}

float error()
{
	return _sp - _pv[(_index - 1 + SAMPLES) % SAMPLES];
}

float integrateError()
{
	//Array is a ring of measurements
	auto start = _measurements == SAMPLES ? _index : 0;

	float error = 0;
	for (int i = 0, j = start; i < _measurements - 1; i++, j = (j + 1) % SAMPLES)
	{
		//Invariant: error = integrate(start, j, pv over dt)
		auto timeStep = _times[(j + 1) % SAMPLES] - _times[j];
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
	auto error1 = _sp - _pv[(_index - 1 + SAMPLES) % SAMPLES];
	auto error2 = _sp - _pv[(_index - 2 + SAMPLES) % SAMPLES];

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

