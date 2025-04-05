#include "kernel/types.h"
#include "user/user.h"

#define INTEGER_SIZE sizeof(int)

void primes(int* pip){

    int p = 0, q = 0;
    int pid;

    while(read(pip[0],&p,INTEGER_SIZE) < INTEGER_SIZE);
    close(pip[1]);
    printf("primes %d\n",p);

    int new_pip[2];
    
    if(pipe(new_pip) < 0){
        fprintf(2,"created pipe failed!\n");
        exit(1);        
    }

    pid = fork();

    if(pid == 0){    /* child process */
        primes(new_pip);
        exit(0);
    }else{    /* parant process */
        while(read(pip[0],&q,INTEGER_SIZE) >= INTEGER_SIZE){
            if(q % p){
                write(new_pip[1],&q,INTEGER_SIZE);
            }
        }
        close(new_pip[0]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char* argv[]){

    int pip[2];
    int pid;

    if(pipe(pip) < 0){
        fprintf(2,"created pipe failed!\n");
        exit(1);
    }

    pid = fork();

    if(pid == 0){   /* child process */
        primes(pip);
        exit(0);
    }else{    /* parent process */
        close(pip[0]); // disable read
        for(int i=2; i<=35; ++i){
            write(pip[1],&i,INTEGER_SIZE);
        }
        wait(0);
        exit(0);
    }
}