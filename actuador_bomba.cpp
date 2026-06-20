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
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
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
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    double generado = randomUniform();
    double caudalActual = *(double *) x.value;
    if(caudalActual > 0 && generado <= PAjusteCaudal){
        ultimaMedicion = caudalActual;
        // generar latencia aleatoria truncada entre 0 y latenciaMaxima
        double proxAjuste;
        do{
            proxAjuste = randomExponential(mediaLatencia);
        } while(proxAjuste > latenciaMaxima);
        
        sigma = proxAjuste;
    } else if(caudalActual > 0 && generado > PAjusteCaudal){
        // double proxAjuste;
        // do{
        //     proxAjuste = randomExponential(mediaLatencia);
        // } while(proxAjuste > latenciaMaxima);
        
        // sigma = proxAjuste; // cambio de especificacion
        sigma = latenciaMaxima; // simula que el actuador intenta corregir hasta el tiempo maximo de latencia
    } else if(caudalActual == 0) {
        ultimaMedicion = 0;
        sigma = 0;
    } else {
        sigma = sigma - e;
    }
}
Event actuador_bomba::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&ultimaMedicion, CAUDAL_ACTUAL_PUERTO);
}
void actuador_bomba::exit() {
//Code executed at the end of the simulation.

}
