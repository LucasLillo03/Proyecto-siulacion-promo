//CPP:mySource/Proyecto-siulacion-promo/sen_confirmacion_enfermero.cpp
#if !defined sen_confirmacion_enfermero_h
#define sen_confirmacion_enfermero_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class sen_confirmacion_enfermero: public Simulator { 
// Declare the state,
// output variables
// and parameters
#include "tipos.h"
#include "constantes.h" 

AlarmaCaudal notificacionAlarma;

double sigma;  	

double confirmacionEnfermero; 


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
