#include "sen_flujo.h"
void sen_flujo::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

caudalMedido = var_arg(parameters, double); 
sigma = var_arg(parameters, double); 

 
}
double sen_flujo::ta(double t) {
//This function returns a double.
return sigma; 	
}
void sen_flujo::dint(double t) {
caudalMedido = //valor random generado a partir del caudal actual en una distribucion normal;
sigma = periodoMuestreoFlujo; 
}
void sen_flujo::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
caudalMedido = //valor random generado a partir del caudal de entrada en una distribucion normal; 
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
