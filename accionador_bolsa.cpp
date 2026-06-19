#include "accionador_bolsa.h"

#define PUERTO_CORRECCION_CAUDAL 0
#define PUERTO_DETENER_BOMBA 1

void accionador_bolsa::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
sigma = inf;
salida = 0;
}
double accionador_bolsa::ta(double t) {
//This function returns a double.
    return sigma;
}
void accionador_bolsa::dint(double t) {
    sigma = inf;
}
void accionador_bolsa::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if(x.port == PUERTO_CORRECCION_CAUDAL){
        salida = *(RangoCaudalRecetado*)x.value;
    } else if(x.port == PUERTO_DETENER_BOMBA){
        salida = 0;
    }
    sigma = 0;
}
Event accionador_bolsa::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&salida, 0);
}
void accionador_bolsa::exit() {
//Code executed at the end of the simulation.

}
