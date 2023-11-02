#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXBUF 1024

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(2, "Usage: %s command [options]\n", argv[0]);
        exit(1);
    }

    char *cmd = argv[1];
    char *cmd_argv[MAXARG];
    cmd_argv[0] = cmd;

    char buf[MAXBUF];
    char *p = buf;
    int cmd_argc = 1;

    while (gets(p, MAXBUF - (p - buf)) != 0) {
        char *q = p;
        while (1) {
            char *s = strchr(q, ' ');
            if (s == 0) {
                break;
            }
            *s = 0;
            if (*q != 0) {
                cmd_argv[cmd_argc] = q;
                cmd_argc++;
            }
            q = s + 1;
        }

        if (*q != 0) {
            cmd_argv[cmd_argc] = q;
            cmd_argc++;
        }
        cmd_argv[cmd_argc] = 0;

        if (fork() == 0) {
            exec(cmd, cmd_argv);
            fprintf(2, "exec %s failed\n", cmd);
            exit(1);
        } else {
            wait(0);
        }

        cmd_argc = 1;
        p = buf;
    }

    return 0;
}
