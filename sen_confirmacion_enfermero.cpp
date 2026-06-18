#include "sen_confirmacion_enfermero.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void sen_confirmacion_enfermero::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

mediaConfirmacion = va_arg(parameters, double);
desvioConfirmacion = va_arg(parameters, double);

notificacionAlarma = ALARMA_CAUDAL_APAGADA; 
confirmacionEnfermero = true;
sigma = inf;
srand(42);

 

}
double sen_confirmacion_enfermero::ta(double t) {
//This function returns a double.
return sigma; 
}
void sen_confirmacion_enfermero::dint(double t) {
sigma = inf; 
}
void sen_confirmacion_enfermero::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

double valorAlarma = *(AlarmaCaudal*)x.value; 
	if (valorAlarma == ALARMA_CRITICA){
		//se usa el algoritmo de box-muller para generar la distribucion normal
	    //TODO modularizar la formula de la distribucion normal
		double u1 = (double)(rand() + 1) / ((double)RAND_MAX + 1);
		double u2 = (double)rand() / (double)RAND_MAX;
		double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

		sigma = mediaConfirmacion + desvioConfirmacion * z;
	}
else{
	sigma = inf; 
}
}
Event sen_confirmacion_enfermero::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&sigma, 0);

// PARA DEBUG

// return Event(&desvioConfirmacion,0);
// return Event(&mediaConfirmacion,0);
}
void sen_confirmacion_enfermero::exit() {
//Code executed at the end of the simulation.

}
