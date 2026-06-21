#include "accionador_bolsa.h"

#define PUERTO_ESTADO_CAUDAL 0
#define PUERTO_CORRECCION_CAUDAL 1
#define PUERTO_DETENER_BOMBA 2

void accionador_bolsa::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);

sigma = INF_VAL;
salida = 0.0;
ultimaSalida = 0.0;

bolsaEstable = true;
estadoEstable = true;

}
double accionador_bolsa::ta(double t) {
//This function returns a double.
    return sigma;
}

void accionador_bolsa::dint(double t) {
    sigma = INF_VAL;
}

void accionador_bolsa::dext(Event x, double t) {
    // bool confirmacionRecibida = false;

    //puerto estado del caudal
    if (x.port == PUERTO_ESTADO_CAUDAL){ 
        bool estadoEntrante = *(bool*)x.value; 

        // if (!estadoEstable && estadoEntrante) {
        //     printLog("confirmacion recibida\n");
        //     confirmacionRecibida = true;
        // }

        estadoEstable = estadoEntrante; 
    }
    // 
    if(x.port == PUERTO_CORRECCION_CAUDAL){
        RangoCaudalRecetado caudalEntrante = *(RangoCaudalRecetado*)x.value;
        
        ultimaSalida = caudalEntrante;
    } 
    else if(x.port == PUERTO_DETENER_BOMBA){
        bool estadoEntrante = *(bool*)x.value; 

        bolsaEstable = estadoEntrante;

    }
    
    if (estadoEstable && bolsaEstable){
        salida = ultimaSalida;
        // if (confirmacionRecibida) {printLog("accionador_bolsa salida %.2f\n", salida);}
    }
    else{
        salida = 0.0;
    }
    sigma = 0;
}

Event accionador_bolsa::lambda(double t) {
    // printLog("salida %.2f: accionador_bolsa %.2f \n", t, salida);
    return Event(&salida, 0);
}

void accionador_bolsa::exit() {
//Code executed at the end of the simulation.

}
