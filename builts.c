#include "minish.h"

//COMANDOS INTERNOS - BEGIN
int builtin_exit (int argc, char ** argv) {
    //Se usa la funcion atoi(), si el string en argv[1] tiene un numero, lo castea a int. pero si se le pasa un string sin numeros, retorna 0. Posteriormente se devuelve ese numero en la syscall.
    if (argc > 2) {
        printf("Los parametros ingresados no son correctos. Luego de exit solo puede haber un numero.\n");
        return -1;
    } else {
        if (argc == 2) {
            int a = atoi(argv[1]);
            exit(a);
            return a;
        } else {
            exit(0);
            return 0;
        }
    }
}
int builtin_help (int argc, char ** argv) {
    if (argc == 1) {
        printf("----------------------------------------------------------\n");
        printf("Los comandos internos que existen son:\n");
        for(int i=0; i<12; i++) {					//NUMERO MAGICO, 12 es la cantidad de comandos internos.
            printf("\t%s\n",builtin_arr[i].cmd);
        }
        printf("----------------------------------------------------------\n");
        return 0;
    } else if (argc == 2) {
        printf("----------------------------------------------------------\n");
        struct builtin_struct *builtin_struct_Pedida = builtin_lookup(argv[1]);
        if (builtin_struct_Pedida != NULL) {
            printf("El comando interno %s funciona asi:\n",argv[1]);
            printf("\t%s\n",builtin_struct_Pedida->help_txt);
        } else {
            printf("El comando interno %s no existe\n",argv[1]);
        }
        printf("----------------------------------------------------------\n");
        return 0;
    } else {
        printf("----------------------------------------------------------\n");
        printf("Flaco, se te fueron los argumentos. Uno o dos nomas\n");
        printf("----------------------------------------------------------\n");
        return SOBRAN_ARGUMENTOS;
    }

}
int builtin_history (int argc, char ** argv) {

    //--------------obtenemos el destino del historial------------------
    char direccion[256];
    strcat(strcpy(direccion, getenv("HOME")), "/.minish_history");
    //------------------------------------------------------------------

    FILE *fp;


    fp = fopen(direccion, "r");


    if(fp == NULL) {
        perror("Failed: ");
        return 1;
    }

    int qimprimir = 10;

    if(argc>1) {
        qimprimir = atoi(argv[1]);
    }

    int lineas = 0;
    for (char elemento = fgetc(fp); elemento != EOF; elemento = fgetc(fp)) { //contamos la cantidad de lineas
        if (elemento == '\n') {
            lineas++;
        }
    }

    //printf("Lineas: %d\n",lineas);

    rewind(fp);//volvemos al inicio del archivo

    int aux=0;

    while(aux < (lineas - qimprimir)) { //llegamos hasta la primer linea a imprimir
        if(fgetc(fp) == '\n') {
            aux++;
        }
    }

    char e;

    while( (e=getc(fp))!=EOF ) { //imprimimos
        putchar(e);
    }

    fclose(fp);//cerramos el arhivo

    return 0;

}
int builtin_status (int argc, char ** argv) {
    printf("Status: %d\n",last_status);
    return 0;
}
int builtin_cd (int argc, char ** argv) {
    if(argc > 2) {
        printf("Muchos parametros\n");
        return -1;
    }

    if(argc == 1) {
        //ir a HOME
        if(getenv("HOME")!=NULL) {
            chdir(getenv("HOME"));
            return 0;
        } else {
            printf("La variable HOME no existe\n");
            return -1;
        }
    }

    if(strcmp(argv[1], "-") == 0) {
        char *atras = "..";
        chdir(atras);
        return 0;
    } else {
        DIR* dir = opendir(argv[1]);
        if(dir) {
            //si existe el directorio
            chdir(argv[1]);
            return 0;
        }
        else if (ENOENT == errno) {
            //si no existe el directorio
            printf("El directorio no existe\n");
            return -1;
        } else {
            //otro error
            printf("error");
            return -1;
        }
    }

    return 0;
}
int builtin_dir (int argc, char ** argv) {
    if(argc>2) {
        printf("Flaco, Muchos argumentos\n");
        return SOBRAN_ARGUMENTOS;
    }

    //Se devuelve un puntero que apunta a dirent, a la carpeta donde es llamada la syscall opendir
    DIR *dp;
    struct dirent *ep;
    dp = opendir("./");

    //Se evalua que el puntero devuelto no sea nulo y genere un core dumped
    if (dp != NULL) {
        if (argc==1) {
            //El funcionamiento de este while es analogo a cuando haciamos while((c=getchar()) != 'EOF'), en este caso se itera entre los archivos que hay en la carpeta dir
            while((ep = readdir(dp))) {
                puts(ep->d_name);
            }
        } else {	//Si entra aca argc solo puede ser 2
            int noHayPalabra = 1;
            while((ep = readdir(dp))) {
                //En el caso que dir tenga un parametro, entra en juego la funcion strstr. En esta funcion se comparan dos strings, y si se encuentra
                //la palabra del segundo argumento, en algun lugar de la palabra del primer argumento, la funcion devuelve un puntero al char del string
                //del primer argumento, donde se encontro el substring. Si no se encontro la palabra, entonces devuelve NULL.
                if (strstr(ep->d_name, argv[1])!= NULL) {
                    puts(ep->d_name);
                    noHayPalabra = 0;
                }
            }
            if(noHayPalabra == 1) {
                printf("No existe ningun archivo con \"%s\".\n", argv[1]);
            }
        }
        closedir(dp);
    } else {
        perror("No hay directorio");//Se imprime el texto y se modifica errno
    }

    return 0;
}
int builtin_getenv (int argc, char ** argv) {
    if (argc == 1) {
        printf("Flaco, te faltan argumentos. Que banderas queres saber?\n");
        return 1;
    } else {
        for (int i=1; i<argc; i++) {
            if (getenv(argv[i]) != NULL) {
                printf("La variable de entorno %s es: %s.\n",argv[i],getenv(argv[i]));
            } else {
                printf("La variable de entorno %s no fue encontrada.\n",argv[i]);
            }
        }
    }
    return 0;
}
int builtin_gid (int argc, char ** argv) {
    //El ID del GID principal
    printf("El GID principal es: %d\n",getgid());


    //Los ID del los GID secundarios
    __uid_t uid = getuid();//El uid del usuario

    //Con ese uid, se saca los GID
    struct passwd* pw = getpwuid(uid);
    if(pw == NULL) {
        perror("getpwuid error: ");
    }

    int numeroDegrupos = 0;

    //Se llama a la syscall para saber la cantidad de grupos
    getgrouplist(pw->pw_name, pw->pw_gid, NULL, &numeroDegrupos);
    __gid_t grupos[numeroDegrupos];

    //Se obtienen los grupos y se guardan en el array grupos
    getgrouplist(pw->pw_name, pw->pw_gid, grupos, &numeroDegrupos);


    //Se recorren los grupos y se imprimen
    for (int i = 1; i < numeroDegrupos; i++) {		//Arranca en 1 ya que el primero es el GID principal
        struct group* gr = getgrgid(grupos[i]);
        if(gr == NULL) {
            perror("getgrgid error: ");
        }
        printf("\tSecundario: %s con ID %d.\n",gr->gr_name,gr->gr_gid);
    }

    return 0;
}
int builtin_setenv (int argc, char ** argv) {
    if (argc == 1) {
        printf("Flaco, te faltan argumentos. Nombre de las banderas que queres modificar o crear, y su valor.\n");
        return 1;
    } else if(argc == 2) {
        printf("Flaco, te faltan argumentos. Nombre de las banderas que queres modificar o crear, o su valor. Hay algo mal, Fijate.\n");
        return 2;
    } else if (argc == 3) {
        return setenv(argv[1],argv[2],1);
    } else {
        printf("Flaco, se fue fueron los argumentos. Es el comando, una variable y su valor.\n");
        return -40;
    }
}
int builtin_pid (int argc, char ** argv) {
    int a = getpid();
    printf("Process ID: %d\n",a);
    return 0; //Siempre devuelve 0, al igual como indica el manual
}
int builtin_uid (int argc, char ** argv) {
    int uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    if(pwd == NULL) {
        perror("getpwuid error: ");
        return -1;
    }
    printf("User Id: %d\n", pwd->pw_uid);
    printf("User Name: %s\n", pwd->pw_name);
    return 0;
}
int builtin_unsetenv (int argc, char ** argv) {
    int a=0;

    for(int i=0; i<argc; i++) {
        a = a + unsetenv(argv[i]);
    }
    if(a!=0) {
        return -1;
    }
    else {
        return 0;
    }
}
//COMANDOS INTERNOS - END

//Se crea la estructura built_in_struct de todos los comandos internos - BEGIN
struct builtin_struct builtin_arr[] = {
    {"exit", builtin_exit, HELP_EXIT},
    {"help", builtin_help, HELP_HELP},
    {"history", builtin_history, HELP_HISTORY},
    {"status", builtin_status, HELP_STATUS},
    {"cd", builtin_cd, HELP_CD},
    {"dir", builtin_dir, HELP_DIR},
    {"getenv", builtin_getenv, HELP_GETENV},
    {"gid", builtin_gid, HELP_GID},
    {"setenv", builtin_setenv, HELP_SETENV},
    {"pid", builtin_pid, HELP_PID},
    {"uid", builtin_uid, HELP_UID},
    {"unsetenv", builtin_unsetenv, HELP_UNSETENV},
    {NULL, NULL, NULL}
};
//Se crea la estructura built_in_struct de todos los comandos internos - END
