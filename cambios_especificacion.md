Sensor de flujo: 
    Se cambio la implementacion del delta interno y externo agregando una condicion en la cual si el flujo medido es menor igual a 0 este se mantenga asi hasta que llegue un nuevo flujo, de esta manera evitamos alteraciones para cuando la bomba este detenida.
    Ahora al modificar el flujo se toma el valor absoluto para evitar resultados negativos. 

Sensor de fin bolsa: 
    Se cambio el tipo de la variable de estado contenidoRestante de RangoCaudalRecetado a double.
    Se agrego un parametro "capacidadMaxima" para definir el contenido inicial de la bolsa.
    Se corrigieron las condiciones teniendo en cuenta tambien el tiempo pasado en el estado y la division por 3600.
    

Actuador de bomba:
    En la especificacion, cuando el actuador no podia ajustar el caudal tardaba un tiempo sigma_ajuste en generar la salida. lo mas correcto (nuestra interpretacion) es que se simule que el actuador realiza el intento de corregir hasta que llega el tiempo maximo esperado por el sistema y lo deja de intentar. Nuevos parametros de simulacion: mediaLatenciaActuador (default 0.25)

Modulo de alarmas: 
    Fue necesario modificar la estructura del delta externo para adaptarse a las limitaciones de tipado de c++. 
    No usamos un nuevo idle para AlarmaCaudal, utilizamos ALARMA_CAUDAL_APAGADA. 

Desvio caudal: 
    Se cambio el flujo de lambda para adapatarse a las limitaciones de c++. 
    Se agrego el estado intermedio CAUDAL_TOLERANCIA_DESVIO entre NORMAL y DESVIADO.
    Se corrigio la condicion de deteccion de desvio cambiando <= por < para evitar falsos positivos con caudal recetado = 0.
    Se agrego la variable sistemaDetenido que ignora lecturas del sensor cuando el sistema esta detenido (luego de alarma critica).
    Ahora al recibir una nueva orden medica se resetea estadoCaudal a CAUDAL_NORMAL.
    Se protegio sigma y sigma_ec con std::max(0.0, ...) para evitar valores negativos.
    La confirmacion del enfermero ahora verifica sistemaDetenido en lugar de estadoCaudal == CAUDAL_CRITICO.
    Se agrego un delay en ta() para evitar el throw cuando sigma_ec == sigma.

    REFACTORIZACION DEL DESVIO CAUDAL: 
    se separo el DEVS de desvio caudal en dos componentes:
        - El primero del mismo nombre desvio_Caudal_v2 se encarga de decidir si el caudal esta desviado emitiendo dos tipos de salidas: salidaDesvio y salidaCorreccion.
            La primera salida es booleana y con cada lectura nueva devuelve true si se detecto un desvio o false en caso contrario.
            La segunda salida devuelve el caudal recetado en caso de haber un desvio o una nueva receta, este posteriormente se utilizara para corregir dicho desvio. 
        - El segundo DEVS, gestor_estados, es el encargado de determinar en que estado se encuentra el sistema, ya sea normal, desviado o critico. Este consta de dos salidas: 
            La primera dirigira alarmas al modulo de alarmas en casos concretos.
            La segunda se encarga de determinar si hay que detener o no la bomba (en caso de estar en estado critico) esta devuelve un booleano al accionador de la bolsa,
            siendo true si la bomba puede continuar y false en caso contrario. 

Accionador de la bolsa: 
    Se modifico el comportamiento de este componente, ahora tiene tres entradas tal que:
        0- recibe estadoEstable, un booleano enviado desde el gestor de estados, es true si el estado es estable y false si es critico.
        1- recibe correccionCaudal, un double que indica que se debe corregir el caudal con el valor enviado, este valor es enviado por desvio_caudal.
        2- recibe bolsaEstable, un booleano que es true si la bolsa no esta vacia y false en caso contrario. 
    El DEVS simula una compuerta AND con los puertos 0 y 2, cada vez que recibe un valor por dichos puertos actualiza el valor de sus variables internas. Cuando un valor
    entra por el puerto 1 este actualiza la salida con el valor ingresado. Asi mismo cuando cualquier valor entra por cualquier puerto, el sistema ejecuta la condicion 
    (estadoEstable && bolsaEstable), si esta resulta verdadera devuelve el valor de salida real, caso contrario devuelve 0. 

Manejador de la bolsa: 
    Se contemplo un nuevo caso en el que recibe como entrada un estado de bolsa normal y por lo tanto debe continuar la ejecucion.

Sensor de fin bolsa: 
    Se agrego una entrada para la confirmacion del enfermero. Al llegar una confirmacion y si el estado no es normal, la bomba se recarga y vuelve al estado normal.