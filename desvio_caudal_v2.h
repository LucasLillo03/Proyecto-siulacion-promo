//CPP:mysources/desvio_caudal_v2.cpp
#if !defined desvio_caudal_v2_h
#define desvio_caudal_v2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"
#include "random_utils.h"


class desvio_caudal_v2: public Simulator { 
// Declare the state,
// output variables
// and parameters

RangoCaudalRecetado caudalRecetado;
double caudalActual;
double sigma;

double tiempoMaximoInicio;
double desvioMaximo; 

bool correccionPendiente; 

bool salidaDesvio;
bool nuevaOrden;
double salidaCorreccion;

public:
	desvio_caudal_v2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event seleccionarSalida(double t);
	Event lambda(double);
	void exit();
};
#endif

