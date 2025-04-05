#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXBUF 1024

int main(int argc, char *argv[]) {
    
    if(argc < 2){
        fprintf(2,"too few argument for xargs \"system\" call!\n");
        exit(1);
    }

    char buf[MAXBUF] = {0};
    char* buf_p = buf;
    char* cmd[MAXARG] = {0};
    int p = 0;
    int cnt = 0;
   
    for(int i=1; i<argc; ++i){
        cmd[i-1] = argv[i];
    }

    if((cnt = read(0,buf,MAXBUF-1)) == 0){
        exit(0);
    }else if(cnt < 0){
        fprintf(2,"xargs read stdin failed\n");
        exit(1);
    }

    buf[cnt] = 0;

    while(p < cnt){
        for(; buf[p] != '\n'; ++p);

        buf[p] = 0;
        cmd[argc-1] = buf_p;
        ++p;
        buf_p = buf + p;

        if(fork() == 0){
            exec(cmd[0],cmd);
            fprintf(2,"exec() failed\n");
            exit(1);
        }else{
            wait(0);
        }
    }
    
    exit(0);
}
