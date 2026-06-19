#include "desvio_caudal.h"
#include <stdexcept>
#include <math.h>
//inputs ports
#define PUERTO_ORDEN_MEDICA 0 //RangoCaudalRecetado
#define PUERTO_CONFIRMACION_ENFERMERO 1 //bool
#define PUERTO_SENSOR_FLUJO 2 //double

//outputs ports
#define PUERTO_ALARMA 0
#define PUERTO_CORRECCION 1

void desvio_caudal::init(double t,...) {
    //The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
    //To get a parameter: %Name% = va_arg(parameters,%Type%)
    //where:
    //      %Name% is the parameter name
    //	%Type% is the parameter type

    tiempoMaximoInicio = va_arg(parameters, double);
    tiempoMaximoDesvio = va_arg(parameters, double);
    tiempoMedioHastaCritico = va_arg(parameters, double);
    desvioMaximo = va_arg(parameters, double);

    caudalRecetado = 0;
    caudalActual = caudalRecetado; 
    estadoCaudal = CAUDAL_NORMAL;
    sigma_ec = inf;
    sigma = inf; 

    correccionPendiente = false;

    salidaAlarma = ALARMA_CAUDAL_APAGADA;
}
double desvio_caudal::ta(double t) {
    //This function returns a double.
    return (sigma_ec < sigma)? sigma_ec : sigma; 
}
void desvio_caudal::dint(double t) {
    if(correccionPendiente){ 
        sigma = 0; 
        return; 
    }    
    else if (sigma_ec < sigma && estadoCaudal == CAUDAL_NORMAL){
        estadoCaudal = CAUDAL_DESVIADO;
        sigma = inf;
        sigma_ec = tiempoMedioHastaCritico;
    } 
    else if(sigma_ec < sigma && estadoCaudal == CAUDAL_DESVIADO){
        estadoCaudal = CAUDAL_CRITICO;
        sigma = 0;
        sigma_ec = inf;
    }
    else if(sigma < sigma_ec){
        sigma = inf; 
    }
    else{
         throw std::runtime_error("Error: Caso de Desvio Caudal no admitido.");
    }
}
void desvio_caudal::dext(Event x, double t) {
    double demoraInicio = randomUniformConMaximo(tiempoMaximoInicio);

    //Entrada de orden medica
    if (x.port == PUERTO_ORDEN_MEDICA){
        RangoCaudalRecetado valorEntrante = *(RangoCaudalRecetado*)x.value;

        caudalRecetado = valorEntrante;
        sigma = demoraInicio; 
        sigma_ec = inf;
    }

    //entrada sensor de flujo
    else if (x.port = PUERTO_SENSOR_FLUJO){
        double valorEntrante = *(double*)x.value;

        if (caudalRecetado * desvioMaximo <= abs(caudalRecetado - caudalActual)){
            if(estadoCaudal == CAUDAL_NORMAL){
                caudalActual = valorEntrante;
                sigma = 0;
                sigma_ec = tiempoMaximoDesvio;
            }

            else{
                caudalActual = valorEntrante;
                sigma = 0;
                sigma_ec = sigma_ec - e;
            }
        }

        else if(estadoCaudal != CAUDAL_CRITICO){
            caudalActual = valorEntrante;
            estadoCaudal = CAUDAL_NORMAL;
            sigma = 0;
            sigma_ec = inf;
        }
    }
    //entrada de confirmacion del enfermero
    else if (x.port = PUERTO_CONFIRMACION_ENFERMERO){
        if (estadoCaudal == CAUDAL_CRITICO){
            estadoCaudal = CAUDAL_NORMAL;
            sigma = 0; 
            sigma_ec = inf;
        }
    }

    //default
    else{
        sigma = sigma - e;
        sigma_ec = sigma_ec - e;
    }
}

Event desvio_caudal::seleccionarSalida(double t){
    if (!correccionPendiente){ 
        return Event(&salidaAlarma, PUERTO_ALARMA);
    }
    else{
        return Event(&caudalRecetado, PUERTO_CORRECCION);
    }
}
Event desvio_caudal::lambda(double t) {
    if (estadoCaudal == CAUDAL_NORMAL){ 
        salidaAlarma = ALARMA_CAUDAL_APAGADA;
        return seleccionarSalida(t);
    }

    else if(estadoCaudal == CAUDAL_DESVIADO){ 
        salidaAlarma = ALARMA_MEDIA;
        return seleccionarSalida(t);
    }

    else if(estadoCaudal == CAUDAL_CRITICO){
        caudalRecetado = 0; 
        salidaAlarma = ALARMA_CRITICA;
        return seleccionarSalida(t);
    }
    else{
        throw std::runtime_error("Error: Caso no contemplado en lambda desvio caudal.");    
    }
}
void desvio_caudal::exit() {
//Code executed at the end of the simulation.

}
