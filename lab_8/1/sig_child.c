#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <PID_родителя>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t ppid = (pid_t)atoi(argv[1]);

    if (kill(ppid, SIGUSR1) == 0) {
        printf("Процесс-потомок (PID %d): успешно послал SIGUSR1 родителю (PID %d)\n", getpid(), ppid);
    } else {
        perror("Не удалось послать сигнал SIGUSR1");
    }

    return 0;
}