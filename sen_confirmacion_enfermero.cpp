#include "sen_confirmacion_enfermero.h"
#include "random_utils.h"
#include <math.h>

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
sigma = INF_VAL;

 

}
double sen_confirmacion_enfermero::ta(double t) {
//This function returns a double.
return sigma; 
}
void sen_confirmacion_enfermero::dint(double t) {
sigma = INF_VAL; 
}
void sen_confirmacion_enfermero::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

double valorAlarma = *(AlarmaCaudal*)x.value; 
	if (valorAlarma == ALARMA_CRITICA){
		sigma = std::abs(randomNormal(mediaConfirmacion, desvioConfirmacion));
	}
else{
	sigma = INF_VAL; 
}
}
Event sen_confirmacion_enfermero::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&confirmacionEnfermero, 0);

// PARA DEBUG

// return Event(&desvioConfirmacion,0);
// return Event(&mediaConfirmacion,0);
}
void sen_confirmacion_enfermero::exit() {
//Code executed at the end of the simulation.

}
