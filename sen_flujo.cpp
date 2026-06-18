#include "sen_flujo.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

caudalMedido = 0; 
sigma = periodoMuestreoFlujo;

 

 
}
double sen_flujo::ta(double t) {
//This function returns a double.
return sigma; 	
}
void sen_flujo::dint(double t) {
    //TODO modularizar la formula de la distribucion normal
    double u1 = (double)(rand() + 1) / ((double)RAND_MAX + 1);
    double u2 = (double)rand() / (double)RAND_MAX;
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    caudalMedido = caudalMedido + desvioCaudal * z;
    sigma = periodoMuestreoFlujo;   
}
void sen_flujo::dext(Event x, double t) {
    double valorCaudal = *(double*)x.value; 

    //TODO modularizar la formula de la distribucion normal
    double u1 = (double)(rand() + 1) / ((double)RAND_MAX + 1);
    double u2 = (double)rand() / (double)RAND_MAX;
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    caudalMedido = valorCaudal + desvioCaudal * z; 
    
    sigma = sigma - e;  
}
Event sen_flujo::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&caudalMedido, 0);
}
void sen_flujo::exit() {
//Code executed at the end of the simulation.

}
