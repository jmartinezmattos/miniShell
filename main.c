#include "minish.h"
int last_status = 0;
int argc;

void signal_handler(int sig) {
    printf(" Signal number: %d\n", sig);
    argc = 0;
    return;
}

int add_history(int argc, char ** argv, FILE *fp) {

    for(int i=0; i<argc; i++) {
        fprintf(fp, "%s", argv[i]);
        fprintf(fp, "%s", " ");//ponemos un espacio entre cada argumento
    }

    fprintf(fp, "%c", '\n');

    return 0;
}

int main() {

    char cwd[200];
    char entrada[200];
    struct sigaction act_INT;

    //----Cancelo la funcion de ctrl-C///
    sigaction(SIGINT, NULL, &act_INT);
    act_INT.sa_handler = signal_handler;
    sigaction(SIGINT, &act_INT, NULL);
    //------------------------------///

    //--------------obtenemos el destino del historial------------------
    char direccion[256];
    strcat(strcpy(direccion, getenv("HOME")), "/.minish_history");
    //------------------------------------------------------------------

    FILE *fp;


    char **argv = NULL;

    argv = (char **) malloc(MAXWORDS);

    while(1) {

        *argv = NULL;

        argc = 5;

        printf("%s> ", getcwd(cwd, sizeof(cwd)) );

        fgets(entrada,200,stdin); //fgets retorna el string con un /n al final ej: hola/n/0

        if(feof(stdin)) {
            printf("Ctrl + D detectado, saliendo de minishell.\n");
            exit(0);
        }

        argc = linea2argv(entrada, argc, argv);

        if(argc >=1 ) {

            fp = fopen(direccion, "a");

            add_history(argc,argv,fp);

            fclose(fp);

            last_status = ejecutar(argc, argv);
        }
    }

    exit(0);

}

