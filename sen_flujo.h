//CPP:mySource/Proyecto-siulacion-promo/sen_flujo.cpp
#if !defined sen_flujo_h
#define sen_flujo_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class sen_flujo: public Simulator { 
// Declare the state,
// output variables
// and parameters

double caudalMedido; 
double sigma; 

double sensorFlujo;

double periodoMuestreoFlujo; 
public:
	sen_flujo(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
