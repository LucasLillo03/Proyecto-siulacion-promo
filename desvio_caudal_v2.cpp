#include "desvio_caudal_v2.h"
#include <stdexcept>
#include <math.h>
#include "random_utils.h"

//inputs ports
#define PUERTO_ORDEN_MEDICA 0 //RangoCaudalRecetado
#define PUERTO_SENSOR_FLUJO 1 //double

//outputs ports
#define PUERTO_DESVIO 0
#define PUERTO_CORRECCION 1

void desvio_caudal_v2::init(double t,...) {
    //The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
   
    tiempoMaximoInicio = va_arg(parameters, double);
    desvioMaximo = va_arg(parameters, double);

    caudalRecetado = 0;
    caudalActual = caudalRecetado; 
    sigma = INF_VAL; 

    correccionPendiente = false;
    salidaDesvio = false;
    salidaCorreccion = caudalRecetado;
}
double desvio_caudal_v2::ta(double t) {
    return sigma; 
}
void desvio_caudal_v2::dint(double t) {
    if(correccionPendiente){ 
        sigma = 0;  
    }    
    else{
        sigma = INF_VAL; 
    }
}
void desvio_caudal_v2::dext(Event x, double t) {
    //Entrada de orden medica
    if (x.port == PUERTO_ORDEN_MEDICA){
        double demoraInicio = randomUniformConMaximo(tiempoMaximoInicio);

        RangoCaudalRecetado valorEntrante = *(RangoCaudalRecetado*)x.value;
        caudalRecetado = valorEntrante;
    }


    //entrada sensor de flujo
    else if (x.port == PUERTO_SENSOR_FLUJO && !sistemaDetenido){
        double valorEntrante = *(double*)x.value;

        if (caudalRecetado * desvioMaximo < abs(caudalRecetado - valorEntrante)){
            salidaDesvio = true;
            sigma = 0; 
        }
        else (
            salidaDesvio = false; 
            sigma = 0;
        )
    }
}

Event desvio_caudal_v2::seleccionarSalida(double t){
    if (!correccionPendiente){ 
        correccionPendiente = true;
        return Event(&salidaAlarma, PUERTO_DESVIO);
    }
    else{
        correccionPendiente = false;
        return Event(&salidaCorreccion, PUERTO_CORRECCION);
    }
}
Event desvio_caudal_v2::lambda(double t) {
    if (salidaDesvio){
        salidaCorreccion = caudalRecetado; 
        return seleccionarSalida(t);
    }
    else{
        return Event(&salidaAlarma, PUERTO_DESVIO);
        //no hay que corregir caudal
    }
}
void desvio_caudal_v2::exit() {
//Code executed at the end of the simulation.

}
