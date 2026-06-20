#include "sen_fin_bolsa.h"
#define FIN_BOLSA 0 

void sen_fin_bolsa::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
margenBolsa = va_arg(parameters, double);
capacidadMaxima = va_arg(parameters, double);

contenidoRestante = capacidadMaxima;
estado = NORMAL;
sigma = INF_VAL;
}
double sen_fin_bolsa::ta(double t) {
//This function returns a double.
return sigma;
}
void sen_fin_bolsa::dint(double t) {
sigma = INF_VAL;
}
void sen_fin_bolsa::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
double valorEntrada = (*(double*)x.value * e) / 3600; // se divide por 3600 ya que el caudal esta en ml/h y el tiempo en segundos
if (contenidoRestante - valorEntrada <= margenBolsa && contenidoRestante > margenBolsa){
    contenidoRestante = contenidoRestante - valorEntrada;
    estado = AGOTANDOSE; 
    printLog("estado AGOTANDOSE \n");
    sigma = 0;
}
else if (contenidoRestante - valorEntrada <= 0 && contenidoRestante > 0){
    contenidoRestante = 0;
    estado = VACIA;
    sigma = 0; 
}
else{
    contenidoRestante = contenidoRestante - valorEntrada;
    sigma = INF_VAL; 
}
}
Event sen_fin_bolsa::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event(&estado, FIN_BOLSA);
}
void sen_fin_bolsa::exit() {
    printLog("contenidoFinal = %f \n", contenidoRestante);
}
