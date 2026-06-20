//CPP:mysources/actuador_bomba.cpp
#if !defined actuador_bomba_h
#define actuador_bomba_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "tipos.h"
#include "constantes.h"


class actuador_bomba: public Simulator { 
// Declare the state,
// output variables
// and parameters
double sigma;
RangoCaudalRecetado ultimaMedicion;
double latenciaMaxima;
double mediaLatencia;
double PAjusteCaudal;
public:
	actuador_bomba(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
