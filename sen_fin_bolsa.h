//CPP:mysources/sen_fin_bolsa.cpp
#if !defined sen_fin_bolsa_h
#define sen_fin_bolsa_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"



class sen_fin_bolsa: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sensorFlujo;

double contenidoRestante;
EstadoBolsa estado;
double sigma;

double margenBolsa;
double capacidadMaxima;

public:
	sen_fin_bolsa(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
