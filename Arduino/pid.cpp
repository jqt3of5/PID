#include <math.h>
#include "pid.h"

double Kp = 4;
double Ki = 0.1;
double Kd = 10;

double Td = 20;
double Ti = 20;

double _sp = 0;

double _pv;
double _time;
double integratedError = 0;
double derivedError = 0;

void tune(double kp, double ki, double kd)
{
	Kp = kp;
	Ki = ki;
	Kd = kd;
}

void setPoint(double sp)
{
	_sp = sp;
}

void setProcessVariable(double pv, double time)
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

double error()
{
	return _sp - _pv;
}

double getManipulatedVariable()
{
	auto p = Kp * error();
	auto i = Ki/Ti * integratedError;
	auto d = Kd * Td* derivedError;

	return p + i + d;
}

void setIntegralWindupMode(WindupMode mode)
{

}
