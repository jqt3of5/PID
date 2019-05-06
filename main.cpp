// PID.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include "Arduino/pid.h"
#include "analysis.h"

float readTemp();

bool heating = false;
float timeStep = 1.0 / 50.0;

int main()
{
	start(1000);
	setPoint(1000);
	
	auto temp = readTemp();
	log(0, temp);
	setProcessVariable(temp, 0);

	for (int i = 1; i < 29000000; ++i)
	{
		//Time steps to heat/cool
		auto mv = getManipulatedVariable();

		if (mv > 100)
		{
			mv = 100;
		}

		if (mv < 0)
		{
			mv = 0;
		}

		heating = true;
		for (int j = 0; j < mv; ++j, ++i)
		{
			temp = readTemp();

			log(i * timeStep, temp);
			setProcessVariable(temp, i * timeStep);
			
		}

		heating = false;
		for (int j = 0; j < 100 - mv; ++j, ++i)
		{
			temp = readTemp();

			log(i * timeStep, temp);
			setProcessVariable(temp, i * timeStep);
//			printf("%f %f\n", i * timeStep, temp);
		}
		printf("%f %f\n", i * timeStep, temp);
	}


	float timeToMax,  avgPeriod,  avgAmplitude;
	analyse(timeToMax, avgPeriod, avgAmplitude);

	printf("%f %f %f", timeToMax, avgPeriod, avgAmplitude);
}


//Heating: temp(t) = heatingRate*t + ambient
//Cooling: temp(t) = coolingFactor/t + ambient
//dtemp/temp = heatingRate - (temp - ambient)^2/coolingFactor


float ambientTemp = 20; //Celcius
float maxAttainableTemp = 2000; //Celcius, point where ability to heat is equal to dissipated heat
float heatingRate = 1;//Celcius per Second, perfect insulation

float coolingFactor = (maxAttainableTemp - ambientTemp) * (maxAttainableTemp - ambientTemp) / heatingRate;

float heatingdt(float temp)
{
	return heatingRate;
}

float coolingdt(float temp)
{
	auto a = (temp - ambientTemp);
	return - a * a / coolingFactor;
}

float readTemp()
{
	static float temp = ambientTemp;

	float t = coolingdt(temp);
	if (heating)
	{
		t += heatingdt(temp);
	}
	
	temp += t * timeStep;

	return temp;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
