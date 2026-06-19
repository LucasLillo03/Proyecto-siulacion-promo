#include "gen_alarmas.h"
void gen_alarmas::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
sigma = 0;
alarm.tipo = ORIGEN_CAUDAL;
alarm.caudal = ALARMA_CRITICA;
}
double gen_alarmas::ta(double t) {
//This function returns a double.
return sigma;
}
void gen_alarmas::dint(double t) {
sigma = 1000;
}
void gen_alarmas::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event gen_alarmas::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&alarm, 0);
}
void gen_alarmas::exit() {
//Code executed at the end of the simulation.

}
