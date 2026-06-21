#include "modulo_alarmas.h"
#define PUERTO_NOTIFICACION_ALARMA 0
#define PUERTO_ALARMA_BAJA 1

void modulo_alarmas::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

ac = ALARMA_CAUDAL_APAGADA;
sigma_ac = INF_VAL;

ab = IDLE_ALARMA_FIN_BOLSA;
sigma_ab = INF_VAL;

estadoCritico = IDLE_CRITICO;

tiempoConfirmacionCritica = va_arg(parameters, double);
repeticionAlarmaCritica = va_arg(parameters, double);
}
double modulo_alarmas::ta(double t) {
    return (sigma_ac < sigma_ab) ? sigma_ac : sigma_ab;
}
void modulo_alarmas::dint(double t) {
    if (ab == ALARMA_BAJA){
        ab = IDLE_ALARMA_FIN_BOLSA;
        sigma_ab = INF_VAL;
    }
    else if(ab != ALARMA_BAJA && ac == ALARMA_MEDIA){
        ac = ALARMA_CAUDAL_APAGADA;
        sigma_ac = INF_VAL;
    }
    else if(ab != ALARMA_BAJA && ac == ALARMA_CRITICA && estadoCritico == CONFIRMANDO){
        ac = ALARMA_CRITICA;
        sigma_ac = repeticionAlarmaCritica;
    } 
    else{
        ac = ALARMA_CRITICA;
        sigma_ac = tiempoConfirmacionCritica;
        estadoCritico = CONFIRMANDO;
    }
}
void modulo_alarmas::dext(Event x, double t) {
    double alarmaRecibida = *(double*)x.value;
    if(alarmaRecibida == VALOR_ALARMA_APAGADA) {
            ac = ALARMA_CAUDAL_APAGADA;
            sigma_ac = INF_VAL;
            estadoCritico = IDLE_CRITICO;
    } else if(alarmaRecibida == VALOR_ALARMA_CRITICA || alarmaRecibida == VALOR_ALARMA_MEDIA){
        ac = alarmaRecibida == VALOR_ALARMA_CRITICA ? ALARMA_CRITICA : ALARMA_MEDIA;
        sigma_ac = 0;
        printLog("entrada %.2f: alarma de caudal %s\n", t, (ac == ALARMA_MEDIA) ? "MEDIA" : "CRITICA");
    } else if(alarmaRecibida == VALOR_ALARMA_BAJA) {
        sigma_ac = (sigma_ac > e) ? sigma_ac - e : 0.0;
        sigma_ab = 0;
        ab = ALARMA_BAJA;
    }
}
Event modulo_alarmas::lambda(double t) {
    if (ab == ALARMA_BAJA){
        salida = VALOR_ALARMA_BAJA;
        return Event(&salida, PUERTO_ALARMA_BAJA);
    }
    else{
        if(ac == ALARMA_CAUDAL_APAGADA) salida = VALOR_ALARMA_APAGADA;
        else if(ac == ALARMA_MEDIA) salida = VALOR_ALARMA_MEDIA;
        else salida = VALOR_ALARMA_CRITICA;

        return Event(&salida, PUERTO_NOTIFICACION_ALARMA);
    }
}
void modulo_alarmas::exit() {
//Code executed at the end of the simulation.

}
