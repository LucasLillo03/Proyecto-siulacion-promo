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