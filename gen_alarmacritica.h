//CPP:mysources/gen_alarmacritica.cpp
#if !defined gen_alarmacritica_h
#define gen_alarmacritica_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "../mysources/tipos.h"
#include "../mysources/constantes.h"


class gen_alarmacritica: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
AlarmaCaudal alarm;
public:
	gen_alarmacritica(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
