#include <math.h>


void analyse(float target,int count, float times[], float value[], float & timeToMax, float & period, float & amplitude)
{
	bool idle = true, heating = false, cooling = false;

	int index = 0;
	int criticalPoints[100] = { 0 };

	int startedHeating = -1;
	int maxTemp = -1;
	
	float dt = 0;
	for (int i = 1; i < count; ++i)
	{
		dt = (value[i] - value[i - 1]) / (times[i] - times[i - 1]);
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


	timeToMax = times[maxTemp] - times[startedHeating];
	period = 0;
	amplitude = 0;
	for (int i = 1; i < index; ++i)
	{
		auto a = criticalPoints[i-1];
		auto b = criticalPoints[i];
		period += times[b] - times[a];

		amplitude += fabsf(value[b] - value[a]);
	}

	period = period / index / 2.0;
	amplitude = amplitude / index;
}

