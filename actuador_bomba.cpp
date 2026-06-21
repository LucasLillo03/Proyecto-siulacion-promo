#include "actuador_bomba.h"
#include "constantes.h"
#include "random_utils.h"
//entrada
#define AJUSTAR_CAUDAL_PUERTO 0 

//salida
#define CAUDAL_ACTUAL_PUERTO 0 

void actuador_bomba::init(double t,...) {
    //The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);

    sigma = 0;
    ultimaMedicion = 0;
    latenciaMaxima = getScilabVar("latenciaActuador");
    mediaLatencia = getScilabVar("mediaLatenciaActuador");
    PAjusteCaudal = getScilabVar("PAjusteCaudal");
}
double actuador_bomba::ta(double t) {
//This function returns a double.
return sigma;
}
void actuador_bomba::dint(double t) {
sigma = INF_VAL;
}
void actuador_bomba::dext(Event x, double t) {
    double generado = randomUniform();
    double caudalActual = *(double *) x.value;
    // printLog("entrada %.2f: actuador_bomba %.2f \n", t, caudalActual);

    // si el valor entrante es positivo y la probabilidad de correccion es verdadera
    if(caudalActual > 0 && generado <= PAjusteCaudal){
        
        
        ultimaMedicion = caudalActual;
        // generar latencia aleatoria truncada entre 0 y latenciaMaxima
        double proxAjuste;
        do{
            proxAjuste = randomExponential(mediaLatencia);
        } while(proxAjuste > latenciaMaxima);
        
        sigma = proxAjuste;
    } 

    else if(caudalActual > 0 && generado > PAjusteCaudal){
        // double proxAjuste;
        // do{
        //     proxAjuste = randomExponential(mediaLatencia);
        // } while(proxAjuste > latenciaMaxima);
        
        // sigma = proxAjuste; // cambio de especificacion
        // printLog("no corrigio \n");

        sigma = latenciaMaxima; // simula que el actuador intenta corregir hasta el tiempo maximo de latencia
    } 

    else if(caudalActual == 0) {
        ultimaMedicion = 0;
        sigma = 0;
    } 

    else {
        sigma = sigma - e;
    }
}
Event actuador_bomba::lambda(double t) {
    // printLog("salida %.2f: ajuste de caudal realizado. valor actual: %.2f \n", t, ultimaMedicion);
    return Event(&ultimaMedicion, CAUDAL_ACTUAL_PUERTO);
}
void actuador_bomba::exit() {
//Code executed at the end of the simulation.

}
