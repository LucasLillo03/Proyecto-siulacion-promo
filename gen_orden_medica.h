//CPP:mySources.cpp
//CPP:mysources/gen_orden_medica.cpp
#if !defined gen_orden_medica_h
#define gen_orden_medica_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "tipos.h"
#include <cmath>



class gen_orden_medica: public Simulator { 
// Declare the state,
// output variables
// and parameters
// Variables de Estado
double actual;
double sigma;

// Variable de salida
double y; 

// Par�metros de simulaci�n
double PCaudalNulo;
double sigma_arr;
double mediaCaudal;





public:
	gen_orden_medica(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
