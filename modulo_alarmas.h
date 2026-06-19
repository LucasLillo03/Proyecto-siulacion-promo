//CPP:mysources/modulo_alarmas.cpp
#if !defined modulo_alarmas_h
#define modulo_alarmas_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"


class modulo_alarmas: public Simulator { 
// Declare the state,
// output variables
// and parameters

AlarmaCaudal ac;
double sigma_ac;

AlarmaFinBolsa ab;
double sigma_ab;

EstadoCritico estadoCritico; 

double tiempoConfirmacionCritica;
double repeticionAlarmaCritica;

public:
	modulo_alarmas(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
