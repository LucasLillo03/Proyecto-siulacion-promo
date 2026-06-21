//CPP:mysources/set_semilla.cpp
#if !defined set_semilla_h
#define set_semilla_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "constantes.h"


class set_semilla: public Simulator { 
// Declare the state,
// output variables
// and parameters
double semilla;
public:
	set_semilla(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
