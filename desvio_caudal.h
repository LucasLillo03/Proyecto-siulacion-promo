//CPP:mysources/desvio_caudal.cpp
#if !defined desvio_caudal_h
#define desvio_caudal_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"
#include "random_utils.h"


class desvio_caudal: public Simulator { 
// Declare the state,
// output variables
// and parameters

RangoCaudalRecetado caudalRecetado;
double caudalActual;
RangoEstadoCaudal estadoCaudal;
double sigma_ec;
double sigma;

double tiempoMaximoInicio;
double tiempoMaximoDesvio;
double tiempoMedioHastaCritico;
double desvioMaximo; 

bool correccionPendiente; 
bool sistemaDetenido;

Alarmas salidaAlarma;

public:
	desvio_caudal(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event seleccionarSalida(double t);
	Event lambda(double);
	void exit();
};
#endif
