#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2]; // parent send to child
    int p2[2]; // child send to parent
    pipe(p1);
    pipe(p2);

    if (fork() != 0) {
        close(p1[0]);
        char ping = 'a';
        write(p1[1], &ping, 1);
        close(p1[1]);
        close(p2[1]);
        char pong;
        read(p2[0], &pong, 1);
        close(p2[0]);
        printf("%d: received pong\n", getpid());
        exit(0);
    } else {
        close(p1[1]);
        char ping;
        read(p1[1], &ping, 1);
        close(p1[0]);
        printf("%d: received ping\n", getpid());
        close(p2[0]);
        write(p2[1], &ping, 1);
        close(p2[1]);
        exit(0);
    }
}
