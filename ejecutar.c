#include "minish.h"

int ejecutar (int argc, char **argv) {
    if(builtin_lookup(argv[0]) != NULL) { //si encuentra el comando
        return (builtin_lookup(argv[0])->func)(argc,argv);//lo ejecuta
    } else {
        return externo(argc,argv);
    }
}

