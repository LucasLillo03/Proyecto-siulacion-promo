#include "gen_orden_medica.h"
#include "random_utils.h"
void gen_orden_medica::init(double t,...) {
	//The 'parameters' variable contains the parameters transferred from the editor.
	va_list parameters;
	va_start(parameters,t);
	//To get a parameter: %Name% = va_arg(parameters,%Type%)
	//where:
	//      %Name% is the parameter name
	//	%Type% is the parameter type

	PCaudalNulo = getScilabVar("PCaudalNulo");
	sigma_arr = getScilabVar("ordenInterarribo");
	mediaCaudal = getScilabVar("mediaCaudal");

	actual = randomExponential((double)mediaCaudal);
	sigma = 0;
}

double gen_orden_medica::ta(double t) {
	return sigma;
}

void gen_orden_medica::dint(double t) {
	double generado = randomUniform();
	bool simulacionActiva = getScilabVar("simulacionActiva");

	if (!simulacionActiva) {
		sigma = INF_VAL;
		return;
	}

	if(generado <= PCaudalNulo) {
		actual = 0.0;
	} else {
		double nuevoCaudal = randomExponential((double)mediaCaudal);
		
		// if (nuevoCaudal > 200.0) nuevoCaudal = 200.0;
		
		actual = nuevoCaudal;
	}

	sigma = sigma_arr;
}

void gen_orden_medica::dext(Event x, double t) {

}
Event gen_orden_medica::lambda(double t) {
	bool simulacionActiva = getScilabVar("simulacionActiva");
	
	if (!simulacionActiva) {
		return Event(&actual, 1);
	}

	return Event(&actual, 0);
}
void gen_orden_medica::exit() {
	executeScilabJob("simulacionActiva = 1;",true);
}
