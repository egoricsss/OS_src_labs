// sig_parent.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void handler(int sig) {
    signal(sig, SIG_DFL);

    printf("Процесс %d получил сигнал %s (%d)\n", getpid(),
           (sig == SIGUSR1) ? "SIGUSR1" :
           (sig == SIGUSR2) ? "SIGUSR2" : "UNKNOWN", sig);

    signal(sig, handler);
}

int main() {
    pid_t pid;

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGINT, SIG_DFL);   
    signal(SIGCHLD, SIG_IGN); 

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        char parent_pid_str[20];
        snprintf(parent_pid_str, sizeof(parent_pid_str), "%d", getppid());

        execl("./sig_child", "./sig_child", parent_pid_str, NULL);

        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        printf("Родительский процесс (PID %d) ожидает сигналы...\n", getpid());
        while (1) {
            pause();
        }
    }

    return 0;
}
