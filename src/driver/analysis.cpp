#include <math.h>
#include <iostream>

int _index = 0;
float* _times; 
float* _values; 

int _critIndex = 0;
int _criticalPoints[10] = { 0 };

int firstChange = -1;

void analysis_start(float target)
{
	_index = 0;
	_times = (float*)calloc(30000000, sizeof(float));
	_values = (float*)calloc(30000000, sizeof(float));
}

bool analysis_update(float time, float value)
{
	//If the change in temp is small, start the killclock
	static float killClockStart = -1;
	
	static bool heating = false;
	static bool cooling = false;

	_times[_index] = time;
	_values[_index] = value;
	if (_index == 0)
	{
		_index++;
		return true;
	}
	
	auto diff = _values[_index] - _values[_index-1];
	if (diff > 0)
	{
		if (cooling || firstChange == -1)
		{
			//trough
			_criticalPoints[_critIndex] = _index-1;
			_critIndex += 1;
			if (firstChange == -1)
			{
				firstChange = _index;
			}
		}

		
		heating = true;
		cooling = false;
	}
	if (diff < 0)
	{
		if (heating || firstChange == -1)
		{
			//peak
			_criticalPoints[_critIndex] = _index-1;
			_critIndex += 1;
			if (firstChange == -1)
			{
				firstChange = _index;
			}
		}

		cooling = true;
		heating = false;
	}
	if (diff == 0)
	{

	}

	if (fabsf(diff) < 0.001 * value)
	{
		if (killClockStart == -1)
		{
			killClockStart = time;
		}

		//Kill after 2minutes
		if (time - killClockStart > 600)
		{
			return false;
		}
	}
	else
	{
		killClockStart = -1;
	}

	if (_critIndex == 10)
	{
		return false;
	}
	
	_index++;
	return true;
}

void analysis_analyse(float& timeToMax, float& period, float& amplitude)
{
	int maxTempIndex = _critIndex < 2 ? _times[_index-1] : _criticalPoints[1];

	timeToMax = _times[maxTempIndex] - _times[firstChange];
	period = 0;
	amplitude = 0;
	for (int i = 2; i < _critIndex; ++i)
	{
		auto a = _criticalPoints[i-1];
		auto b = _criticalPoints[i];
		period += _times[b] - _times[a];

		amplitude += fabsf(_values[b] - _values[a]);
	}

	period = period / _critIndex / 2.0;
	amplitude = amplitude / _critIndex;
}

