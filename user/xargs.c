#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs cmd ...\n");
        exit(1);
    }
    char *cmdArgv[MAXARG+1];
    int cmdArgc1 = argc - 1;
    for (int i=1; i<argc; i++) {
        cmdArgv[i-1] = argv[i];
    }
    char tmp;
    while (1) {
        if (read(0, &tmp, sizeof(tmp)) == 0) {
            break;
        }
        int cmdArgc2 = cmdArgc1;
        char arg[512];
        int p = 0;
        while (1) {
            if (tmp == '\n' || tmp == ' ') {
                char buf[p+1];
                memcpy(buf, arg, p);
                buf[p] = '\0';
                cmdArgv[cmdArgc2++] = buf;
                p = 0;
                if (tmp == '\n') {
                    cmdArgv[cmdArgc2] = 0;
                    if (fork() == 0) {
                        exec(cmdArgv[0], cmdArgv);
                    } else {
                        wait((int *) 0);
                        break;
                    }
                }
            } else {
                arg[p++] = tmp;
            }
            read(0, &tmp, sizeof(tmp));
        }
    }
    exit(0);
}
