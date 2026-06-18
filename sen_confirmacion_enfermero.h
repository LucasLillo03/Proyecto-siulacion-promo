//CPP:mysources/sen_confirmacion_enfermero.cpp
#if !defined sen_confirmacion_enfermero_h
#define sen_confirmacion_enfermero_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "tipos.h"
#include "constantes.h"


class sen_confirmacion_enfermero: public Simulator { 
// Declare the state,
// output variables
// and parameters

AlarmaCaudal notificacionAlarma;

double sigma;  	

bool confirmacionEnfermero; 

double desvioConfirmacion;
double mediaConfirmacion;




public:
	sen_confirmacion_enfermero(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
