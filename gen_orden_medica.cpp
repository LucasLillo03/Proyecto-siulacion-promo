#include "gen_orden_medica.h"
void gen_orden_medica::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

PCaudalNulo = va_arg(parameters, double);
sigma_arr = va_arg(parameters, double);
mediaCaudal = va_arg(parameters, double);

actual = 0.0;
sigma = sigma_arr;
}
double gen_orden_medica::ta(double t) {
//This function returns a double.
return sigma;
}
void gen_orden_medica::dint(double t) {

double generado = (double)rand() / (double)RAND_MAX;

if(generado <= PCaudalNulo) {
	actual = 0.0;
} else {
	double u = ((double)rand() / (double)RAND_MAX) + 1e-7;
	double nuevoCaudal = -(1/mediaCaudal) * log(u);
	
	if (nuevoCaudal > 200.0) nuevoCaudal = 200.0;
	
	actual = nuevoCaudal;
}

sigma = sigma_arr;
}
void gen_orden_medica::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event gen_orden_medica::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&actual, 0);
}
void gen_orden_medica::exit() {
//Code executed at the end of the simulation.

}
