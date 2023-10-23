#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]){

    int pip[2];
    uchar data = 10;
    int pid;

    // Create two pipes
    if(pipe(pip) < 0){
        fprintf(2,"create pipe failed!\n");
        exit(1);
    }

    if((pid = fork()) < 0){
        fprintf(2, "fork error\n");
        exit(1);
    }

    if(pid == 0){    /* child process */
        while(read(pip[0],&data,1) <= 0);
        close(pip[0]);
        pid = getpid();
        printf("%d: received ping (%d)\n",pid,data);
        data = 30;
        write(pip[1],&data,1);
        close(pip[1]);
        exit(0);
    }else{    /* parent process */
        data = 20;
        write(pip[1],&data,1);
        close(pip[1]);
        wait(0);
        while(read(pip[0],&data,1) <= 0);
        pid = getpid();
        printf("%d: received pong (%d)\n",pid,data);
        close(pip[0]);
        exit(0);
    }
}
