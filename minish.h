#define HISTORY_FILE    ".minish_history"   // nombre del archivo que almacena historia de comandos

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h>
#define MAXLINE 1024        // tamaño máximo de la línea de entrada
#define MAXCWD 1024         // tamaño máximo para alojar el pathname completo del directorio corriente
#define MAXWORDS 256        // cantidad máxima de palabras en la línea
#define FALTAN_ARGUMENTOS -40
#define SOBRAN_ARGUMENTOS -50	
#define IN  1 				/* inside a word */
#define OUT 0 				/*outside a word */
#define MAXLETRAS 1024
#define HELP_CD      "cd [..|dir] - cambia de directorio corriente"
#define HELP_DIR     "dir [str]- muestra archivos en directorio corriente, que tengan 'str'"
#define HELP_EXIT    "exit [status] - finaliza el minish con un status de retorno (por defecto 0)"
#define HELP_HELP    "help [cd|dir|exit|help|history|getenv|pid|setenv|status|uid]"
#define HELP_HISTORY "history [N] - muestra los últimos N (10) comandos escritos"
#define HELP_GETENV  "getenv var [var] - muestra valor de variable(s) de ambiente"
#define HELP_PID     "pid - muestra Process Id del minish" //Inventada
#define HELP_GID     "pid - muestra todos los Group Id del minish"
#define HELP_SETENV  "setenv var valor - agrega o cambia valor de variable de ambiente"
#define HELP_UNSETENV  "unsetenv var - elimina una variable de ambiente" //Inventada
#define HELP_STATUS  "status - muestra status de retorno de ultimo comando ejecutado"
#define HELP_UID     "uid - muestra nombre y número de usuario dueño del minish"


// Variables
extern int last_status;       // guarda status del ultimo comando - deberá definirse en el main

// Funciones
extern int builtin_exit (int argc, char ** argv);
extern int builtin_help (int argc, char ** argv);
extern int builtin_history (int argc, char ** argv);
extern int builtin_status (int argc, char ** argv);
extern int builtin_cd (int argc, char ** argv);
extern int builtin_dir (int argc, char ** argv);
extern int builtin_getenv (int argc, char ** argv);
extern int builtin_gid (int argc, char ** argv);
extern int builtin_setenv (int argc, char ** argv);
extern int builtin_pid (int argc, char ** argv);
extern int builtin_uid (int argc, char ** argv);
extern int builtin_unsetenv (int argc, char ** argv);
extern int ejecutar (int argc, char ** argv);
extern int externo (int argc, char ** argv);
extern int linea2argv(char *linea, int argc, char **argv);

struct builtin_struct {         // struct con información de los builtins
    char *cmd;                  // nombre del comando builtin
    int (*func) (int, char **); // la función que lo ejecuta
    char *help_txt;             // el texto de ayuda
};

extern struct builtin_struct builtin_arr[];
extern struct builtin_struct * builtin_lookup(char *cmd);

