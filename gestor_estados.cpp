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
    salidaCorreccion = true; // true = la bomba puede continuar, false = la bomba debe ser detenida 
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

        // printLog("tolerancia terminada\n");

        estadoCaudal = CAUDAL_CRITICO;
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

        // printLog("entrada %.2f: desvio entrante %d \n", t, caudalDesviado);


        if(caudalDesviado || estadoCaudal == CAUDAL_CRITICO){ 
            if (estadoCaudal == CAUDAL_NORMAL){ 
                // printLog("entrada %.2f: caudal desviado \n", t);

                // printLog("caudal desviandose\n");
                estadoCaudal = CAUDAL_DESVIADO;
                sigma = tiempoMaximoDesvio;
            }
            else{
                // printLog("caudal no corregido\n");

                sigma = std::max(0.0, sigma - e);
            }
        }
        else{
            // printLog("entrada %.2f: caudal corregido\n",t);
            if (estadoCaudal == CAUDAL_CRITICO) {printLog("caudal corregido\n");}


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
        salidaCorreccion = true;
        return seleccionarSalida(t);
    }
    else if (estadoCaudal == CAUDAL_DESVIADO){
        // printLog("salida %.2f: alarma media emitida\n", t);

        salidaAlarma.caudal = ALARMA_MEDIA;
        salidaCorreccion = true;
        return seleccionarSalida(t);
    }
    else if(estadoCaudal == CAUDAL_CRITICO){
        salidaAlarma.caudal = ALARMA_CRITICA;
        salidaCorreccion = false;
        return seleccionarSalida(t); 
    }
}
void gestor_estados::exit() {
//Code executed at the end of the simulation.

}
