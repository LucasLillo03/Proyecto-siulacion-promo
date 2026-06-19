#ifndef TIPOS_BOMBA_H
#define TIPOS_BOMBA_H

// 1. Rango numérico para el caudal
typedef double RangoCaudalRecetado;

// 2. Fases de la bolsa de medicación
enum EstadoBolsa {
    NORMAL,
    AGOTANDOSE,
    VACIA
};

// 3. Niveles de criticidad registrados internamente en el Controlador
enum EstadoAlarma {
    NINGUNA,
    BAJA,
    MEDIA,
    CRITICA
};

// 4. Señales específicas vinculadas a desvíos de flujo
enum AlarmaCaudal {
    ALARMA_MEDIA,
    ALARMA_CRITICA,
    ALARMA_CAUDAL_APAGADA
};

// 5. Señales vinculadas al nivel de la bolsa
enum AlarmaFinBolsa {
    IDLE_ALARMA_FIN_BOLSA,
    ALARMA_BAJA
};

// 6. Conjunto Global Alarmas (Unión de AlarmaCaudal y AlarmaFinBolsa)
// En C++, representamos esta unión matemática con un enum discriminador y una estructura
enum TipoOrigenAlarma {
    ORIGEN_CAUDAL,
    ORIGEN_BOLSA
};

struct Alarmas {
    TipoOrigenAlarma tipo;       // Nos dice qué tipo de alarma contiene esta estructura
    AlarmaCaudal caudal;         // Valor si es una alarma de caudal
    AlarmaFinBolsa bolsa;        // Valor si es una alarma de bolsa
};

// 7. Fases operativas internas del Módulo de Alarmas
enum EstadoCritico {
    IDLE_CRITICO,               // Se usa IDLE_CRITICO para evitar conflictos de nombres en C++
    CONFIRMANDO
};

#endif // TIPOS_BOMBA_H