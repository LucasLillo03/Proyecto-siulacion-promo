//CPP:mysources/manejador_bolsa.cpp
#if !defined manejador_bolsa_h
#define manejador_bolsa_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "tipos.h"
#include "constantes.h"

class manejador_bolsa: public Simulator { 
// Declare the state,
// output variables
// and parameters

	double sigma;
	EstadoManejadorBolsa estadoManejador;
	Alarmas salidaFinBolsa;

public:
	manejador_bolsa(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
