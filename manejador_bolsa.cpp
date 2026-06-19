#include "manejador_bolsa.h"

#define ALARMA_BAJA_PUERTO 0
#define DETENER_BOMBA_PUERTO 1

void manejador_bolsa::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

    sigma = 0;
    ultimaEntrada = VACIA;
}
double manejador_bolsa::ta(double t) {
//This function returns a double.
    return sigma;
}
void manejador_bolsa::dint(double t) {
    sigma = inf;
}
void manejador_bolsa::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    EstadoBolsa entrada = *(EstadoBolsa*) x.value;

    if(entrada == AGOTANDOSE){
        double tiempoMaximoAgote = getScilabVar("tiempoMaximoAgote");
        sigma = tiempoMaximoAgote;
        ultimaEntrada = entrada;
    } else if(entrada == VACIA){
        sigma = 0;
        ultimaEntrada = entrada;
    } else {
        // no deberia pasar
    }
}
Event manejador_bolsa::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    if(ultimaEntrada == AGOTANDOSE){
        salida = ALARMA_BAJA;
        return Event(&salida, ALARMA_BAJA_PUERTO);
    } else if(ultimaEntrada == VACIA){
        double salida = 0;
        return Event(&salida, DETENER_BOMBA_PUERTO);
    }else {
        // no deberia pasar
    }

return Event();
}
void manejador_bolsa::exit() {
//Code executed at the end of the simulation.

}
