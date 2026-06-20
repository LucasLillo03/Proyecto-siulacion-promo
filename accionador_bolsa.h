//CPP:mysources/accionador_bolsa.cpp
#if !defined accionador_bolsa_h
#define accionador_bolsa_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "tipos.h"
#include "constantes.h"


class accionador_bolsa: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
RangoCaudalRecetado salida;

bool bolsaVacia;

public:
	accionador_bolsa(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
