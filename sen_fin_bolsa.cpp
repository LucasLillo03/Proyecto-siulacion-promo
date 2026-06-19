#include "sen_fin_bolsa.h"
void sen_fin_bolsa::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
margenBolsa = va_arg(parameters, double);
capacidadMaxima = va_arg(parameters, double);

contenidoRestante = capacidadMaxima;
estado = NORMAL;
sigma = inf;
}
double sen_fin_bolsa::ta(double t) {
//This function returns a double.
return sigma;
}
void sen_fin_bolsa::dint(double t) {
sigma = inf;
}
void sen_fin_bolsa::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event sen_fin_bolsa::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&estado, 0);
}
void sen_fin_bolsa::exit() {
//Code executed at the end of the simulation.

}
