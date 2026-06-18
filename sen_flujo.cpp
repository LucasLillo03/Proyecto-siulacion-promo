#include "sen_flujo.h"
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
putScilabVar("nuevoCaudalMedidoInterno", caudalMedido);
putScilabVar("desvioCaudal", desvioCaudal);

char comandoScilab[] = "grand(1,1, 'nor', nuevoCaudalMedidoInterno, desvioCaudal)"; 

double nuevoCaudalMedidoInterno = executeScilabJob(comandoScilab,true);

caudalMedido = nuevoCaudalMedidoInterno;
sigma = periodoMuestreoFlujo; 
}
void sen_flujo::dext(Event x, double t) {
    double valorCaudal = *(double*)x.value; 

    putScilabVar("nuevoCaudalMedidoExterno", valorCaudal);
    putScilabVar("desvioCaudal", desvioCaudal);

    char comandoScilab[] = "grand(1,1, 'nor', nuevoCaudalMedidoExterno, desvioCaudal)"; 

    double resultadoScilab = executeScilabJob(comandoScilab, true);

    caudalMedido = resultadoScilab; 
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
