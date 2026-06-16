//CPP:mySources.cpp
//CPP:mysources/gen_orden_medica.cpp
#if !defined gen_orden_medica_h
#define gen_orden_medica_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class gen_orden_medica: public Simulator { 
// Declare the state,
// output variables
// and parameters

// Variables de Estado
double actual;
double sigma;

// Variable de salida
double y; 

// Parámetros de simulación
double PCaudalNulo;
double sigma_arr;


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
