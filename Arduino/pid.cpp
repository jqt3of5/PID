#include <math.h>
#include "pid.h"

double pid_setPoint = 0;
double pid_manipulatedVariable = 0;

double Kp = 4;
double Ki = 0.1;
double Kd = 10;

double Td = 20;
double Ti = 20;


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

void pid_setProcessVariable(double pv, double time)
{
	auto timeStep = time - _time;
	integratedError += (pid_setPoint - pv) * timeStep;

	if (timeStep != 0)
	{
		//derivedError = ((_sp - pv) - (_sp - _pv)) / timeStep;
		derivedError = (_pv - pv) / timeStep;
	}

	_time = time;
	_pv = pv;

	pid_updateManipulatedVariable();
}

double error()
{
	return pid_setPoint - _pv;
}

double pid_updateManipulatedVariable()
{
	auto p = Kp * error();
	auto i = Ki/Ti * integratedError;
	auto d = Kd * Td* derivedError;

	pid_manipulatedVariable = p + i + d;
	return pid_manipulatedVariable;
}

void setIntegralWindupMode(WindupMode mode)
{

}
