#include "minish.h"

int externo(int argc, char **argv) {

    pid_t pid = fork();

    if(pid == -1) {
        perror("fork");
    }
    else if(pid == 0) {
        //child
        if(execvp(argv[0], argv)==-1) {
            perror("exec");
            char cwd[200];
            printf("%s> ", getcwd(cwd, sizeof(cwd)) );
        }
        exit(0);
    }
    else if(pid>0) {
        //parent
        //char cwd[200];
        //printf("%s> ", getcwd(cwd, sizeof(cwd)) );
    }
    return 0;
}

