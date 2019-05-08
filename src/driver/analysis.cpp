#include <math.h>
#include <iostream>

int _index = 0;
float* _times; 
float* _values; 

void start(float target)
{
	_index = 0;
	_times = (float*)calloc(30000000, sizeof(float));
	_values = (float*)calloc(30000000, sizeof(float));
}

void log(float time, float value)
{
	_times[_index] = time;
	_values[_index] = value;
	_index++;
}

void analyse(float& timeToMax, float& period, float& amplitude)
{
	bool idle = true, heating = false, cooling = false;

	int index = 0;
	int criticalPoints[100] = { 0 };

	int startedHeating = -1;
	int maxTemp = -1;
	
	float dt = 0;
	for (int i = 1; i < _index; ++i)
	{
		dt = (_values[i] - _values[i - 1]) / (_times[i] - _times[i - 1]);
		if (dt > 0)
		{
			if (startedHeating == -1)
			{
				startedHeating = i;
			}

			if (cooling)
			{		
				if (index < 100)
				{
					criticalPoints[index] = i;
					index++;
				}
			}

			heating = true;
			cooling = false;
		}
		if (dt < 0)
		{
			if (maxTemp == -1)
			{
				maxTemp = i;
			}

			if (heating)
			{
				if (index < 100)
				{
					criticalPoints[index] = i;
					index++;
				}
			}

			heating = false;
			cooling = true;
		}
		
	}


	timeToMax = _times[maxTemp] - _times[startedHeating];
	period = 0;
	amplitude = 0;
	for (int i = 1; i < index; ++i)
	{
		auto a = criticalPoints[i-1];
		auto b = criticalPoints[i];
		period += _times[b] - _times[a];

		amplitude += fabsf(_values[b] - _values[a]);
	}

	period = period / index / 2.0;
	amplitude = amplitude / index;
}

