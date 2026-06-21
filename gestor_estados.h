//CPP:mysources/gestor_estados.cpp
#if !defined gestor_estados_h
#define gestor_estados_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"
#include "random_utils.h"


class gestor_estados: public Simulator { 
// Declare the state,
// output variables
// and parameters

RangoEstadoCaudal estadoCaudal;
double sigma;

double tiempoMaximoDesvio;
double tiempoMedioHastaCritico;

bool correccionPendiente; 

Alarmas salidaAlarma;
double salida;
bool salidaCorreccion;

public:
	gestor_estados(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event seleccionarSalida(double t);
	Event lambda(double);
	void exit();
};
#endif
