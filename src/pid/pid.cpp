#include <math.h>
#include "pid.h"
#include <stdlib.h>

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


PIDState * pid_init(double maxOutput, double minOutput, WindupMode mode)
{
	PIDState * state = (PIDState*)calloc(1, sizeof(PIDState));

	state->_minOutput = minOutput;
	state->_maxOutput = maxOutput;
	state->_mode = mode;

	state->_time = 0;

	state->_integratedError = 0;
	switch(mode)
	{
		case Normal:
		case EnableWhenControllable:
			break;

		case CapIntegralTerm:
			state->_integralMax = maxOutput;
			break;

		case EraseHistory:
			state->_historyCount = 10;
			state->_historyIndex = 0;
			state->_history = (double*)calloc(state->_historyCount, sizeof(double));
			break;
	}
	return state;
}

double calculateIntegratedError(PIDState * state, double error, double timeStep)
{
	switch(state->_mode)
	{
		case Normal:
			state->_integratedError += error * timeStep;
			break;

		case EnableWhenControllable:
			if (state->Kp*error < state->_maxOutput)
			{
				state->_integratedError += error * timeStep;
			}
			else
			{
				state->_integratedError = 0;
			}
			break;

		case CapIntegralTerm:
			state->_integratedError += error * timeStep;
			state->_integratedError = band(state->_integratedError, state->_integralMax, -state->_integralMax);
			break;

		case EraseHistory:
			state->_history[state->_historyIndex] = error * timeStep;
			state->_historyIndex = (state->_historyIndex + 1) % state->_historyCount;

			state->_integratedError = 0;
			for (int i = 0; i < state->_historyCount; ++i)
			{
				state->_integratedError += state->_history[state->_historyIndex%state->_historyCount];
			}
			break;
	}

	return state->_integratedError;
}

double pid_update(PIDState * state, double withProcessVariable, double atTime)
{
	auto timeStep = atTime - state->_time;
	state->_time = atTime;

	auto error = state->setPoint - withProcessVariable;
	auto derivedError = (withProcessVariable - state->_pv) / timeStep;
	auto integratedError = calculateIntegratedError(state, error, timeStep);

	state->_pv = withProcessVariable;

	auto p = state->Kp * error;
	auto i = state->Ki * integratedError;
	auto d = state->Kd * derivedError;

	state->manipulatedVariable = band(p + i - d, state->_maxOutput, state->_minOutput);
	return state->manipulatedVariable;
}
