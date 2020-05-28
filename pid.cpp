#include <math.h>
#include "pid.h"
#include <stdlib.h>

double pid_setPoint = 0;
double pid_manipulatedVariable = 0;

static double Kp = 1;
static double Ki = 0;
static double Kd = 0;

double * pid_Kp() { return &Kp; }
double * pid_Ki() { return &Ki; }
double * pid_Kd() { return &Kd; }

double _pv;
double _time;

double _minOutput;
double _maxOutput;

//Mode to handle integral windup
WindupMode _mode;
//Normal = 0,
//Doesn't modify the integration term at all, sums it up as is.
double _integratedError;

//EnableWhenControllable = 1,
//When the proportional term starts to wane, kick in the integration term

//CapIntegralTerm = 2,
//Cap the integration at the maximum output
double _integralMax;

//EraseHistory = 3;
//Keep a history of measurements and integrate over that history
double * _history;
int _historyCount;
int _historyIndex;
inline double max(double a, double b)
{
	return a > b ? a : b;
}

inline double min(double a, double b)
{
	return a < b ? a : b;
}

inline double band(double n, double maxVal, double minVal)
{
	return max(minVal, min(maxVal, n));
}


void pid_init(double maxOutput, double minOutput, WindupMode mode)
{
	_minOutput = minOutput;
	_maxOutput = maxOutput;
	_mode = mode;

	_time = 0;

	_integratedError = 0;
	switch(mode)
	{
		case Normal:
		case EnableWhenControllable:
			break;

		case CapIntegralTerm:
			_integralMax = maxOutput;
			break;

		case EraseHistory:
			_historyCount = 10;
			_historyIndex = 0;
			_history = (double*)calloc(_historyCount, sizeof(double));
			break;
	}
}

double calculateIntegratedError(double error, double timeStep)
{
	static double integratedError = 0;
	switch(_mode)
	{
		case Normal:
			integratedError += error * timeStep;
			break;

		case EnableWhenControllable:
			if (Kp*error < _maxOutput)
			{
				integratedError += error * timeStep;
			}
			else
			{
				integratedError = 0;
			}
			break;

		case CapIntegralTerm:
			integratedError += error * timeStep;
			integratedError = band(integratedError, _integralMax, -_integralMax);
			break;

		case EraseHistory:
			_history[_historyIndex] = error * timeStep;
			_historyIndex = (_historyIndex + 1) % _historyCount;

			integratedError = 0;
			for (int i = 0; i < _historyCount; ++i)
			{
				integratedError += _history[_historyIndex%_historyCount];
			}
			break;
	}

	return integratedError;
}

double pid_update(double withProcessVariable, double atTime)
{
	auto timeStep = atTime - _time;
	_time = atTime;

	auto error = pid_setPoint - withProcessVariable;
	auto derivedError = (withProcessVariable - _pv) / timeStep;
	auto integratedError = calculateIntegratedError(error, timeStep);

	_pv = withProcessVariable;

	auto p = Kp * error;
	auto i = Ki * integratedError;
	auto d = Kd * derivedError;

	pid_manipulatedVariable = band(p + i - d, _maxOutput, _minOutput);
	return pid_manipulatedVariable;
}
