#include "minish.h"

struct builtin_struct* builtin_lookup(char *cmd) {

    for(int i=0; i<12; i++) {			//NUMERO MAGICO, 12 es la cantidad de comandos internos.
        if(strcmp(builtin_arr[i].cmd,cmd) == 0) {
            return &builtin_arr[i];
        }
    }
    return NULL;
}
