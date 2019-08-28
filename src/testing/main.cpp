// PID.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifdef TESTING
#include <iostream>
#include <math.h>
#include "../pid/pid.h"
#include "analysis.h"

float readTemp();

bool heating = false;
bool cooling = false;
float timeStep = 1.0 / 50.0;

int main()
{
	analysis_start(1000);
	auto state = pid_init(255, 0, EnableWhenControllable);

	state->setPoint = 1000;
	state->Kp = 15;
	state->Ki = .5;
	state->Kd = 100;

	auto temp = readTemp();
	
	pid_update(state, temp, 0);

	int i = 1;
	//run the analysis on each iteration, it can decide when to quit the simulation
	while (analysis_update(i * timeStep, temp))
	{
		printf("%f %f\n", i * timeStep, temp);
		//Time steps to heat/cool
		auto mv = pid_update(state, temp, i * timeStep);
		
		heating = true;
		for (int j = 0; j < mv; ++j, ++i)
		{
			temp = readTemp();
		}

		heating = false;
		for (int j = 0; j < 255 - mv; ++j, ++i)
		{
			temp = readTemp();
		}
	}


	float timeToMax,  avgPeriod,  avgAmplitude;
	analysis_analyse(timeToMax, avgPeriod, avgAmplitude);

	printf("%f %f %f", timeToMax, avgPeriod, avgAmplitude);
}


//Heating: temp(t) = heatingRate*t + ambient
//Cooling: temp(t) = coolingFactor/t + ambient
//dtemp/temp = heatingRate - (temp - ambient)^2/coolingFactor


float ambientTemp = 20; //Celcius
float maxAttainableTemp = 2000; //Celcius, point where ability to heat is equal to dissipated heat
float heatingRate = 1; //Celcius per Second, perfect insulation
float deadTime = 10; //Time it takes output to be measured
float coolingFactor = (maxAttainableTemp - ambientTemp) * (maxAttainableTemp - ambientTemp) / heatingRate;

float heatingdt(float temp)
{
	return heatingRate;
}

float neutraldt(float temp)
{
	auto a = (temp - ambientTemp);
	return -a * a / coolingFactor;
}

float coolingdt(float temp)
{
	return 0;
}

float readTemp()
{
	//Implement dead time simulation
	static int index = 0;
	static int sampleCount = deadTime / timeStep + 1;
	static float* history = new float[sampleCount] {ambientTemp};

	static float time = 0;

	float temp = history[index];

	//Get the natural change
	float dtemp = neutraldt(temp);

	//if the heater is active, add the heat change
	if (heating)
	{
		dtemp += heatingdt(temp);
	}

	//if the cooler is active, add the cooling change
	if (cooling)
	{
		dtemp += coolingdt(temp);
	}
	
	temp += dtemp * timeStep;
	
	//We'll grab the temp at the next index from the array to be the temp currently being read
	//Then we'll update it with the actual current temp (That will be read in a little while!)
	index = (index + 1) % sampleCount;
	float readTemp = history[index];
	history[index] = temp;

	time += timeStep;
	return readTemp;
}
#endif