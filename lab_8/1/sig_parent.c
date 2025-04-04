// sig_parent.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

// Обработчик сигнала
void handler(int sig) {
    // Восстановление диспозиции по умолчанию
    signal(sig, SIG_DFL);

    printf("Процесс %d получил сигнал %s (%d)\n", getpid(),
           (sig == SIGUSR1) ? "SIGUSR1" :
           (sig == SIGUSR2) ? "SIGUSR2" : "UNKNOWN", sig);

    // Можно снова установить обработчик
    signal(sig, handler);
}

int main() {
    pid_t pid;

    // Установка обработчиков сигналов
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGINT, SIG_DFL);    // Реакция по умолчанию на SIGINT (обычно завершение)
    signal(SIGCHLD, SIG_IGN);   // Игнорировать завершение дочернего процесса

    // Создание дочернего процесса
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Ветка дочернего процесса
        char parent_pid_str[20];
        snprintf(parent_pid_str, sizeof(parent_pid_str), "%d", getppid());

        execl("./sig_child", "./sig_child", parent_pid_str, NULL);

        // Если execl не сработал:
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Родительский процесс
        printf("Родительский процесс (PID %d) ожидает сигналы...\n", getpid());
        // Бесконечное ожидание сигналов
        while (1) {
            pause();
        }
    }

    return 0;
}
