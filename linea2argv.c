#include "minish.h"

int linea2argv (char *linea, int argc, char **argv) {
    //IN = 0 OUT =1

    int nw, state, comilla;

    state = OUT;
    nw = 0;
    comilla = OUT;

    while(*linea!='\n' && nw<argc) {

        if(*linea == '\'' /*|| *linea == '’' || *linea == '‘'*/ || *linea == '"') {
            comilla = !comilla;// si se detecta una comilla se hace operacion not con su valor
        }

        if (comilla == OUT && (*linea == ' ' || *linea == '\n' || *linea == '\t') ) {
            state = OUT;
            *linea = '\0';
        }
        else if (state == OUT) {
            state = IN;
            argv[nw] = linea; //se agrega la palabra en el array de punteros
            ++nw;

        }

        linea++;
    }

    *linea = '\0'; //antes era /n

    argv[nw] = NULL;

    return nw;

}


