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
margenBolsa = getScilabVar("margenBolsa");
capacidadMaxima = getScilabVar("capacidadMaximaBolsa");

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
    double valorEntrada = (*(double*)x.value * e) / 3600; // se divide por 3600 ya que el caudal esta en ml/h y el tiempo en segundos

    if (contenidoRestante - valorEntrada <= margenBolsa && contenidoRestante > margenBolsa){
        contenidoRestante = contenidoRestante - valorEntrada;
        estado = AGOTANDOSE; 
        printLog("%.2f estado AGOTANDOSE \n", t);
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
    return Event(&estado, FIN_BOLSA);
}
void sen_fin_bolsa::exit() {
    printLog("contenidoFinal = %f \n", contenidoRestante);
}
