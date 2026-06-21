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
   
    tiempoMaximoInicio = getScilabVar("tiempoMaximoInicio");
    desvioMaximo = getScilabVar("desvioMaximo");

    caudalRecetado = 0;
    caudalActual = caudalRecetado; 
    sigma = INF_VAL; 

    correccionPendiente = false;
    salidaDesvio = false;
    nuevaOrden = false;
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
        nuevaOrden = true;
        sigma = 0;
        
        salidaDesvio = (caudalRecetado == 0.0) ? false : true;

        printLog("entrada %.2f: nueva orden %.2f \n", t, valorEntrante);
    }

    
    //entrada sensor de flujo
    else if (x.port == PUERTO_SENSOR_FLUJO){

        if (caudalRecetado == 0) {
            sigma = INF_VAL;
            return;
        }
        double valorEntrante = *(double*)x.value;
        // printLog("entrada %.2f: nuevo flujo %.2f \n", t, valorEntrante);

        if (caudalRecetado * desvioMaximo < abs(caudalRecetado - valorEntrante)){
            // printLog("flujo desviado \n", t, valorEntrante);

            salidaDesvio = true;
            sigma = 0; 
        }
        else {
            // printLog("flujo correcto \n", t, valorEntrante);
            salidaDesvio = false; 
            sigma = 0;
        }
    }
}

Event desvio_caudal_v2::seleccionarSalida(double t){
    if (!correccionPendiente){ 
        correccionPendiente = true;
        // printLog("salida %.2f: desvio %d \n", t, salidaDesvio);
        return Event(&salidaDesvio, PUERTO_DESVIO);
    }
    else{
        correccionPendiente = false;
        // printLog("salida %.2f: correccion %.2f \n", t, salidaCorreccion);
        return Event(&salidaCorreccion, PUERTO_CORRECCION);
    }
}
Event desvio_caudal_v2::lambda(double t) {
    if (salidaDesvio){
        salidaCorreccion = caudalRecetado; 
        
        return seleccionarSalida(t);
    }
    else if (nuevaOrden){
        salidaCorreccion = caudalRecetado; 

        if (correccionPendiente) {nuevaOrden = false;}

        return seleccionarSalida(t);
    }
    else{
        return Event(&salidaDesvio, PUERTO_DESVIO);
        //no hay que corregir caudal
    }
}
void desvio_caudal_v2::exit() {
//Code executed at the end of the simulation.

}
