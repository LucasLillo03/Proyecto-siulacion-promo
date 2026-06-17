#include "sen_confirmacion_enfermero.h"
void sen_confirmacion_enfermero::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
notificacionAlarma = ALARMA_CAUDAL_APAGADA; 

sigma = inf;

 

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
if (x.value == ALARMA_CRITICA){
	double tiempoConfirmacion = //TODO tiempo generado en base a una distribucion normal;
	sigma = tiempoConfirmacion;
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


return Event(true, 0);
}
void sen_confirmacion_enfermero::exit() {
//Code executed at the end of the simulation.

}
