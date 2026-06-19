//CPP:mysources/gen_alarmas.cpp
#if !defined gen_alarmas_h
#define gen_alarmas_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"


class gen_alarmas: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
	Alarmas alarm;
public:
	gen_alarmas(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
