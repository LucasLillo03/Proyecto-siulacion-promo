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
    else if(ab != ALARMA_BAJA && ac == ALARMA_CRITICA && estadoCritico == CONFIRMANDO){
        ac = ALARMA_CRITICA;
        sigma_ac = repeticionAlarmaCritica;
    } 
    else if(ab != ALARMA_BAJA && ac == ALARMA_MEDIA){
        ac = ALARMA_CAUDAL_APAGADA;
        sigma_ac = INF_VAL;
    }
    else{
        ac = ALARMA_CRITICA;
        sigma_ac = tiempoConfirmacionCritica;
        estadoCritico = CONFIRMANDO;
    }
}
void modulo_alarmas::dext(Event x, double t) {
    double alarmaRecibida = *(double*)x.value;
    
    if(alarmaRecibida == VALOR_ALARMA_APAGADA){
            ac = ALARMA_CAUDAL_APAGADA;
            sigma_ac = INF_VAL;
            estadoCritico = IDLE_CRITICO;
    }
    else if(alarmaRecibida == VALOR_ALARMA_MEDIA){
            ac = ALARMA_MEDIA;
            sigma_ac = 0;
    }
    else if (alarmaRecibida == VALOR_ALARMA_CRITICA){
            ac = ALARMA_CRITICA;
            sigma_ac = 0;
    }
    else if(alarmaRecibida == VALOR_ALARMA_BAJA){
            sigma_ac = (sigma_ac > e) ? sigma_ac - e : 0.0;
            sigma_ab = 0;
            ab = ALARMA_BAJA;
    }
    else{}
    // if (alarmaRecibida.tipo == ORIGEN_CAUDAL) {
    //     if (alarmaRecibida == VALOR_ALARMA_APAGADA) {
    //         ac = ALARMA_CAUDAL_APAGADA;
    //         sigma_ac = INF_VAL;
    //         estadoCritico = IDLE_CRITICO;
    //     } else {
    //         ac = alarmaRecibida.caudal;
    //         sigma_ac = 0;
    //     }
    // }
    // else if (alarmaRecibida.tipo == ORIGEN_BOLSA && alarmaRecibida.bolsa == ALARMA_BAJA) {
    //     sigma_ac = (sigma_ac > e) ? sigma_ac - e : 0.0;
    //     sigma_ab = 0;
    //     ab = ALARMA_BAJA;
    // }
}
Event modulo_alarmas::lambda(double t) {
    if (ab == ALARMA_BAJA){
        salida_alarmas = VALOR_ALARMA_BAJA;
        return Event(&salida_alarmas, PUERTO_ALARMA_BAJA);
    }
    else{
        if(ac == ALARMA_CRITICA){
            salida_alarmas = VALOR_ALARMA_CRITICA;
        } else if (ac == ALARMA_MEDIA){
            salida_alarmas = VALOR_ALARMA_MEDIA;
        } else{
            salida_alarmas = VALOR_ALARMA_APAGADA;
        }
        return Event(&salida_alarmas, PUERTO_NOTIFICACION_ALARMA);
    }
}
void modulo_alarmas::exit() {
//Code executed at the end of the simulation.

}
