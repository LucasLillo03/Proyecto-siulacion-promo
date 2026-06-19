#include "set_semilla.h"
#include "constantes.h"
void set_semilla::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

    double semilla = getScilabVar("semilla");
    srand(semilla);
}
double set_semilla::ta(double t) {
    return inf;
}
void set_semilla::dint(double t) {

}
void set_semilla::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event set_semilla::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event();
}
void set_semilla::exit() {
//Code executed at the end of the simulation.

}
