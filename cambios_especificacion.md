Sensor de flujo: 
    Se cambio la implementacion del delta interno y externo agregando una condicion en la cual si el flujo medido es menor igual a 0 este se mantenga asi hasta que llegue un nuevo flujo, de esta manera evitamos alteraciones para cuando la bomba este detenida.
    Ahora al modificar el flujo se toma el valor absoluto para evitar resultados negativos. 

Sensor de fin bolsa: 
    Se cambio el tipo de la variable de estado contenidoRestante de RangoCaudalRecetado a double.
    Se agrego un parametro "capacidadMaxima" para definir el contenido inicial de la bolsa.