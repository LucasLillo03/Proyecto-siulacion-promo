#include "gestor_estados.h"
#include <stdexcept>
#include <math.h>
#include "random_utils.h"

//inputs ports
#define PUERTO_CONFIRMACION_ENFERMERO 0 //bool
#define PUERTO_DESVIO 1 //RangoCaudalRecetado

//outputs ports
#define PUERTO_ALARMA 0
#define PUERTO_CORRECCION 1

void gestor_estados::init(double t,...) {
    //The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);

    tiempoMaximoDesvio = va_arg(parameters, double);
    tiempoMedioHastaCritico = va_arg(parameters, double);

    estadoCaudal = CAUDAL_NORMAL;
    sigma = INF_VAL; 

    correccionPendiente = false;
    salidaAlarma.tipo = ORIGEN_CAUDAL;
    salidaAlarma.caudal = ALARMA_CAUDAL_APAGADA;
    salidaCorrecion = true; // true = la bomba puede continuar, false = la bomba debe ser detenida 
}

double gestor_estados::ta(double t) {
    return sigma;
}

void gestor_estados::dint(double t) {
    if (correccionPendiente){
        sigma = 0;
    }
    //si entra con caudal desviado significa que agoto su tiempo de tolerancia
    else if (estadoCaudal == CAUDAL_DESVIADO){
        estadoCaudal == CAUDAL_CRITICO;
        sigma = tiempoMedioHastaCritico; 
    }
    else{
        sigma = INF_VAL;
    }
}

void gestor_estados::dext(Event x, double t) {

    //entrada de confirmacion del enfermero
    if (x.port == PUERTO_CONFIRMACION_ENFERMERO){
        if (estadoCaudal == CAUDAL_CRITICO){
            printLog("entrada %.2f: confirmacion del enfermero \n", t);
            estadoCaudal = CAUDAL_NORMAL;
            sigma = 0; 
        }
        else{
            sigma = std::max(0.0, sigma - e);
        }
    }

    //entrada de desvio
    if (x.port == PUERTO_DESVIO){
        bool caudalDesviado = *(bool*)x.value;

        if(caudalDesviado){ 
            if (estadoCaudal == CAUDAL_NORMAL){ 
                estadoCaudal = CAUDAL_DESVIADO
                sigma = tiempoMaximoDesvio;
            }
            else{
                sigma = std::max(0.0, sigma - e);
            }
        }
        else{
            estadoCaudal = CAUDAL_NORMAL;
            sigma = 0;
        }
    }
}

Event gestor_estados::seleccionarSalida(double t){
    if (!correccionPendiente){ 
        correccionPendiente = true;
        return Event(&salidaAlarma, PUERTO_ALARMA);
    }
    else{
        correccionPendiente = false;
        return Event(&salidaCorreccion, PUERTO_CORRECCION);
    }
}
Event gestor_estados::lambda(double t) {
    if(estadoCaudal == CAUDAL_NORMAL){
        salidaAlarma.caudal = ALARMA_CAUDAL_APAGADA;
        salidaCorrecion = true;
        return seleccionarSalida(t);
    }
    else if (estadoCaudal == CAUDAL_DESVIADO){
        salidaAlarma.caudal = ALARMA_MEDIA;
        salidaCorrecion = true;
        return seleccionarSalida(t);
    }
    else if(estadoCaudal == CAUDAL_CRITICO){
        salidaAlarma = ALARMA_CRITICA;
        salidaCorrecion = false;
        return seleccionarSalida(t); 
    }
}
void gestor_estados::exit() {
//Code executed at the end of the simulation.

}
