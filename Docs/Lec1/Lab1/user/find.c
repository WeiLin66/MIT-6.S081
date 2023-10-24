#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"


int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(2,"too few argument for find \"system\" call!\n");
        exit(1);
    }

    int fd;
    
    fd = open(".",0);

    struct dirent de;
    struct stat st;

    while(read(fd,&de,sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }
        if(strcmp(argv[1],de.name)){
            continue;
        }
        stat(de.name,&st);
        printf("%s %d\n",de.name,st.type);
    }
    close(fd);
    exit(0);
}