#include "sen_flujo.h"
#include <algorithm> 
#include <math.h>
#include "random_utils.h"
#define SENSOR_FLUJO 0

void sen_flujo::init(double t,...) {
    //The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
    //To get a parameter: %Name% = va_arg(parameters,%Type%)
    //where:
    //      %Name% is the parameter name
    //	%Type% is the parameter type
    periodoMuestreoFlujo = va_arg(parameters, double);
    desvioCaudal = va_arg(parameters, double);

    caudalMedido = 0.0; 
    sigma = INF_VAL;

    sistemaDetenido = true;
}

double sen_flujo::ta(double t) {
//This function returns a double.
return sigma; 	
}
void sen_flujo::dint(double t) {
    bool simulacionActiva = getScilabVar("simulacionActiva");

	if (!simulacionActiva) {
		sigma = INF_VAL;
		return;
	}
    
    caudalMedido = caudalMedido <= 0 ? 0 : std::fabs(randomNormal(caudalMedido, desvioCaudal)); //TODO por alguna razon siguen saliendo valores negativos
    sigma = sistemaDetenido ? INF_VAL : periodoMuestreoFlujo;   
    // sigma = periodoMuestreoFlujo;   

}
void sen_flujo::dext(Event x, double t) {
    double valorCaudal = *(double*)x.value; 

    if (sistemaDetenido && valorCaudal > 0) {
        sistemaDetenido = false;
        sigma = 0;
    }
    else if (!sistemaDetenido && valorCaudal <= 0) {
        sistemaDetenido = true; 
    }
    
    // printLog("sen_flujo: nueva correccion %.2f\n", valorCaudal);

    caudalMedido = std::fabs(randomNormal(valorCaudal, desvioCaudal)); 
    
    sigma = std::max(0.0, sigma - e);  
}
Event sen_flujo::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&caudalMedido, SENSOR_FLUJO);
}
void sen_flujo::exit() {
//Code executed at the end of the simulation.

}
