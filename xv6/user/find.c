#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

#define FILE_MAX_LENGTH   512

#if 1
char* fmtname(char *path){
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char* filename, char* path, int len){

    if(path == 0){
        return;
    }

    char buf[FILE_MAX_LENGTH] = {0};
    int fd;
    int filelen = strlen(path);
    struct stat st;
    struct dirent de;
    char* p = buf;

    if((fd = open(path,0)) < 0){
        fprintf(2, "find: can't open %s\n",path);
        return;
    }

    if(fstat(fd,&st) < 0){
        fprintf(2, "find: can't stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            if(memcmp(filename,fmtname(path),len) == 0){
                printf("%s\n",path);
            }
            break;

        case T_DIR:
            if(filelen + 1 + DIRSIZ > FILE_MAX_LENGTH){
                fprintf(2, "find: path too long\n");
                break;
            }
            memmove(buf,path,filelen);
            p = buf + filelen;
            *p++ = '/';
            while(read(fd,&de,sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || !strcmp(de.name,".") || !strcmp(de.name,"..")){
                    continue;
                }
                memmove(p,de.name,DIRSIZ);
                *(p + DIRSIZ) = 0;
                find(filename,buf,len);
            }
            break;

        default:
        break;
    }
    close(fd);
}
#endif

int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(2,"too few argument for find \"system\" call!\n");
        exit(1);
    }

    char* path = argc == 2 ? "." : argv[1];
    char* filename = argc == 2 ? argv[1] : argv[2];

    find(filename,path,strlen(filename));

    exit(0);
}