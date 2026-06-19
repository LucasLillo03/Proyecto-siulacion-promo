#include "desvio_caudal.h"
#include <stdexcept>
#include <math.h>
#include "random_utils.h"

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
    sistemaDetenido = true;
    salidaAlarma = ALARMA_CAUDAL_APAGADA;
}
double desvio_caudal::ta(double t) {
    if (sigma_ec == sigma){
        sigma += 0.1; 
    }
    return (sigma_ec < sigma)? sigma_ec : sigma; 
}
void desvio_caudal::dint(double t) {
    if(correccionPendiente){ 
        sigma = 0; 
        return; 
    }    
    //se agota el tiempo de tolerancia de desvio
    else if (sigma_ec < sigma && estadoCaudal == CAUDAL_TOLERANCIA_DESVIO){
        estadoCaudal = CAUDAL_DESVIADO;
        sigma = inf;
        sigma_ec = tiempoMedioHastaCritico;
    } 
    //se agota el tiempo de tolerancia antes de pasar a critica
    else if(sigma_ec < sigma && estadoCaudal == CAUDAL_DESVIADO){
        estadoCaudal = CAUDAL_CRITICO;
        sigma = 0;
        sigma_ec = inf;
    }
    else if(sigma < sigma_ec){
        sigma = inf; 
    }
    else{
         printLog("Error: Caso de Desvio Caudal no admitido.");
         throw std::runtime_error("Error: Caso de Desvio Caudal no admitido.");
    }
}
void desvio_caudal::dext(Event x, double t) {
    double demoraInicio = randomUniformConMaximo(tiempoMaximoInicio);

    //Entrada de orden medica
    if (x.port == PUERTO_ORDEN_MEDICA){
        RangoCaudalRecetado valorEntrante = *(RangoCaudalRecetado*)x.value;

        caudalRecetado = valorEntrante;
        estadoCaudal = CAUDAL_NORMAL;
        sigma = demoraInicio; 
        sigma_ec = inf;

        printLog("entrada %.2f: NUEVA RECETA %.2f , incio en %.2f\n", t, valorEntrante, demoraInicio);
    }

    //entrada sensor de flujo
    else if (x.port == PUERTO_SENSOR_FLUJO){
        //si el sistema esta detenido se ignoran las entradas del sensor de flujo
        if (sistemaDetenido){
            return;
        }

        double valorEntrante = *(double*)x.value;

        //si el desvio es mayor al permitido, se inicia el proceso de alarma
        if (caudalRecetado * desvioMaximo < abs(caudalRecetado - caudalActual)){
            if(estadoCaudal == CAUDAL_NORMAL){
                printLog("entrada %.2f desvio caudal detectado. valor actual: %.2f, valor recetado: %.2f \n", t, valorEntrante, caudalRecetado);
                estadoCaudal = CAUDAL_TOLERANCIA_DESVIO;
                caudalActual = valorEntrante;
                sigma = 0;
                sigma_ec = tiempoMaximoDesvio;
            }

            else if(estadoCaudal != CAUDAL_CRITICO){
                caudalActual = valorEntrante;
                sigma = 0;
                sigma_ec = std::max(0.0, sigma_ec - e);
            }
        }
        //si no hay desvio o es menor al permitido, se actualiza el caudal actual y se vuelve a estado normal
        else{
            if (estadoCaudal != CAUDAL_NORMAL){
                printLog("entrada %.2f: desvio caudal corregido. valor actual: %.2f, valor recetado: %.2f \n", t, valorEntrante, caudalRecetado);
            }
            caudalActual = valorEntrante;
            estadoCaudal = CAUDAL_NORMAL;
            sigma = 0;
            sigma_ec = inf;
        }
    }
    //entrada de confirmacion del enfermero
    else if (x.port == PUERTO_CONFIRMACION_ENFERMERO){
        printLog("entrada %.2f: confirmacion del enfermero \n", t);
        if (sistemaDetenido){
            estadoCaudal = CAUDAL_NORMAL;
            sigma = 0; 
            sigma_ec = inf;
        }
    }

    //default
    else{
        sigma = std::max(0.0, sigma - e);
        sigma_ec = std::max(0.0, sigma_ec - e);
    }
}

Event desvio_caudal::seleccionarSalida(double t){
    if (!correccionPendiente){ 
        correccionPendiente = true;
        return Event(&salidaAlarma, PUERTO_ALARMA);
    }
    else{
        correccionPendiente = false;
        return Event(&caudalRecetado, PUERTO_CORRECCION);
    }
}
Event desvio_caudal::lambda(double t) {
    if (estadoCaudal == CAUDAL_NORMAL || estadoCaudal == CAUDAL_TOLERANCIA_DESVIO){ 
        sistemaDetenido = false;
        salidaAlarma = ALARMA_CAUDAL_APAGADA;
        return seleccionarSalida(t);
    }

    else if(estadoCaudal == CAUDAL_DESVIADO){ 
        salidaAlarma = ALARMA_MEDIA;
        printLog("salida %.2f: alarma media\n", t);
        return seleccionarSalida(t);
    }

    else if(estadoCaudal == CAUDAL_CRITICO){
        sistemaDetenido = true;
        caudalRecetado = 0; 
        salidaAlarma = ALARMA_CRITICA;
        printLog("salida %.2f: alarma critica\n", t);
        return seleccionarSalida(t);
    }
    else{
        throw std::runtime_error("Error: Caso no contemplado en lambda desvio caudal.");    
    }
}
void desvio_caudal::exit() {
//Code executed at the end of the simulation.

}
