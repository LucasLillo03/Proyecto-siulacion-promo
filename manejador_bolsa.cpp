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

    sigma = INF_VAL;
    estadoManejador = MANEJADOR_NORMAL;
    salidaFinBolsa.tipo = ORIGEN_BOLSA;
    salidaFinBolsa.bolsa = IDLE_ALARMA_FIN_BOLSA;
}
double manejador_bolsa::ta(double t) {
//This function returns a double.
    return sigma;
}
void manejador_bolsa::dint(double t) {
    if (estadoManejador == MANEJADOR_AGOTANDOSE){
        double tiempoMaximoAgote = getScilabVar("tiempoMaximoAgote");
        sigma = tiempoMaximoAgote;
        estadoManejador = MANEJADOR_VACIA;
    }
    else if (estadoManejador == MANEJADOR_VACIA){
        sigma = INF_VAL;
        estadoManejador = MANEJADOR_NORMAL;
    }
    else{
        sigma = INF_VAL;
    }
}
void manejador_bolsa::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    EstadoBolsa entrada = *(EstadoBolsa*) x.value;

    if(entrada == AGOTANDOSE){
        sigma = 0;
        estadoManejador = MANEJADOR_AGOTANDOSE;
    } else if(entrada == VACIA){
        sigma = 0;
        estadoManejador = MANEJADOR_VACIA;
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
    if(estadoManejador == MANEJADOR_AGOTANDOSE){
        printLog("Salida %.2f: ALARMA BAJA\n", t);

        salidaFinBolsa.bolsa = ALARMA_BAJA;
        return Event(&salidaFinBolsa, ALARMA_BAJA_PUERTO);
    } else if(estadoManejador == MANEJADOR_VACIA){
        printLog("Salida %.2f: BOMBA DETENIDA\n", t);

        bool salida = false;
        executeScilabJob("simulacionActiva = 0",true);
        return Event(&salida, DETENER_BOMBA_PUERTO);
    }else {
        // no deberia pasar
    }

return Event();
}
void manejador_bolsa::exit() {
//Code executed at the end of the simulation.

}
