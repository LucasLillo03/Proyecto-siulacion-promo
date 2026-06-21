#include "sen_fin_bolsa.h"
//puertos de entrada
#define PUERTO_CAUDAL 0
#define PUERTO_CONFIRMACION 1
//puertos de salida
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
    if (x.port == PUERTO_CAUDAL){
        double valorEntrada = (*(double*)x.value * e) / 3600; // se divide por 3600 ya que el caudal esta en ml/h y el tiempo en segundos
        
        //si el nuevo contenido es inferior o igual al margen y el anterior no
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
    else if (x.port == PUERTO_CONFIRMACION){
        if(estado != NORMAL){
            contenidoRestante = capacidadMaxima;
            estado = NORMAL;
            sigma = 0;
        }
    }
}
Event sen_fin_bolsa::lambda(double t) {
    return Event(&estado, FIN_BOLSA);
}
void sen_fin_bolsa::exit() {
    printLog("contenidoFinal = %f \n", contenidoRestante);
}
