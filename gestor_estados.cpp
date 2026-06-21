#include "gestor_estados.h"
#include <stdexcept>
#include <math.h>
#include "random_utils.h"

void gestor_estados::registrarEstado(double t) {
    tiempos.push_back(t);
    switch(estadoCaudal) {
        case CAUDAL_NORMAL:   estados.push_back("normal");   break;
        case CAUDAL_DESVIADO: estados.push_back("desviado"); break;
        case CAUDAL_CRITICO:  estados.push_back("critico");  break;
        default:              estados.push_back("desconocido"); break;
    }
}

//inputs ports
#define PUERTO_CONFIRMACION_ENFERMERO 0 //bool
#define PUERTO_DESVIO 1 //RangoCaudalRecetado

//outputs ports
#define PUERTO_ALARMA 0
#define PUERTO_CORRECCION 1

void gestor_estados::init(double t,...) {
    //The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);

    tiempoMaximoDesvio = getScilabVar("tiempoMaximoDesvio");
    tiempoMedioHastaCritico = getScilabVar("tiempoMedioHastaCritico");

    tiempos.clear();
    estados.clear();

    estadoCaudal = CAUDAL_NORMAL;
    registrarEstado(t);
    sigma = INF_VAL; 

    correccionPendiente = false;
    salidaAlarma.tipo = ORIGEN_CAUDAL;
    salidaAlarma.caudal = ALARMA_CAUDAL_APAGADA;
    salidaCorreccion = true; // true = la bomba puede continuar, false = la bomba debe ser detenida 
}

double gestor_estados::ta(double t) {
    return sigma;
}

void gestor_estados::dint(double t) {
    if (correccionPendiente){
        sigma = 0;
    }
    //si entra con caudal desviado significa que agoto su tiempo de tolerancia
    else if (estadoCaudal == CAUDAL_DESVIADO){

        // printLog("tolerancia terminada\n");

        estadoCaudal = CAUDAL_CRITICO;
        registrarEstado(t);
        sigma = tiempoMedioHastaCritico; 
    }
    else{
        sigma = INF_VAL;
    }
}

void gestor_estados::dext(Event x, double t) {
    bool simulacionActiva = getScilabVar("simulacionActiva");
	
	if (!simulacionActiva) {
		return;
	}

    //entrada de confirmacion del enfermero
    if (x.port == PUERTO_CONFIRMACION_ENFERMERO){
        if (estadoCaudal == CAUDAL_CRITICO){
            printLog("entrada %.2f: confirmacion del enfermero \n", t);
            estadoCaudal = CAUDAL_NORMAL;
            registrarEstado(t);
            sigma = 0; 
        }
        else{
            sigma = std::max(0.0, sigma - e);
        }
    }

    //entrada de desvio
    if (x.port == PUERTO_DESVIO){
        bool caudalDesviado = *(bool*)x.value;

        // printLog("entrada %.2f: desvio entrante %d \n", t, caudalDesviado);


        if(caudalDesviado || estadoCaudal == CAUDAL_CRITICO){ 
            if (estadoCaudal == CAUDAL_NORMAL){ 
                // printLog("entrada %.2f: caudal desviado \n", t);

                // printLog("caudal desviandose\n");
                estadoCaudal = CAUDAL_DESVIADO;
                registrarEstado(t);
                sigma = tiempoMaximoDesvio;
            }
            else{
                // printLog("caudal no corregido\n");

                sigma = std::max(0.0, sigma - e);
            }
        }
        else{
            // printLog("entrada %.2f: caudal corregido\n",t);
            if (estadoCaudal == CAUDAL_CRITICO) {printLog("caudal corregido\n");}


            estadoCaudal = CAUDAL_NORMAL;
            registrarEstado(t);
            sigma = 0;
        }
    }
}

Event gestor_estados::seleccionarSalida(double t){
    if (!correccionPendiente){ 
        correccionPendiente = true;
        return Event(&salida, PUERTO_ALARMA);
    }
    else{
        correccionPendiente = false;
        return Event(&salidaCorreccion, PUERTO_CORRECCION);
    }
}
Event gestor_estados::lambda(double t) {
    if(estadoCaudal == CAUDAL_NORMAL){
        salidaAlarma.caudal = ALARMA_CAUDAL_APAGADA;
        salida = VALOR_ALARMA_APAGADA;
        salidaCorreccion = true;
        return seleccionarSalida(t);
    }
    else if (estadoCaudal == CAUDAL_DESVIADO){
        // printLog("salida %.2f: alarma media emitida\n", t);

        salidaAlarma.caudal = ALARMA_MEDIA;
        salida = VALOR_ALARMA_MEDIA;
        salidaCorreccion = true;
        return seleccionarSalida(t);
    }
    else if(estadoCaudal == CAUDAL_CRITICO){
        salidaAlarma.caudal = ALARMA_CRITICA;
        salida = VALOR_ALARMA_CRITICA;
        salidaCorreccion = false;
        return seleccionarSalida(t); 
    }
}
void gestor_estados::exit() {
    FILE* fp = fopen("./../atomics/mysources/outputs/estados.csv", "w");
    if (fp == NULL) {
        printLog("gestor_estados: no se pudo abrir outputs/estados.csv\n");
        return;
    }
    fprintf(fp, "tiempo, estado\n");
    for (size_t i = 0; i < tiempos.size(); i++) {
        fprintf(fp, "%.6f, %s\n", tiempos[i], estados[i].c_str());
    }
    fclose(fp);
    printLog("gestor_estados: %zu estados guardados en outputs/estados.csv\n", tiempos.size());
}
