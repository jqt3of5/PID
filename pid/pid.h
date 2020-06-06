#pragma once


typedef int WindupMode;
const int Normal = 0, EnableWhenControllable = 1, CapIntegralTerm = 2, EraseHistory = 3;

struct PIDState
{
    double setPoint;
    double manipulatedVariable;
    
    double Kp, Ki, Kd;

    bool active;
    
    WindupMode mode;
    double minOutput, maxOutput;

    double _pv;
    double _time;

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
    //TODO: To save on memory, should I use a UNION to mimic polymorphism?
    double * _history;
    int _historyCount;
    int _historyIndex;
};


PIDState * pid_init( double maxOutput = 255, double minOutput = 0, WindupMode mode = Normal);

double pid_update(PIDState * state, double withProcessVariable, double atTime);

char * pid_toString(PIDState * state, char * string);
//Loop cycletime
//deadtime
