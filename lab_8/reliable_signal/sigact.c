#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void user_handler(int sig)
{
    printf("Получен сигнал %s (%d). Засыпаем на 60 секунд...\n", (sig == SIGUSR1) ? "SIGUSR1" : "SIGUSR2", sig);

    sleep(60);

    printf("Good morning!!!\nSignal: %s (%d).\n", (sig == SIGUSR1) ? "SIGUSR1" : "SIGUSR2", sig);
}

void (*mysig(int sig, void (*handler)(int)))(int)
{
    struct sigaction act;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);  
    act.sa_flags = 0;

    if (sigaction(sig, &act, NULL) < 0)
    {
        perror("sigaction");
        return SIG_ERR;
    }

    return act.sa_handler;
}

int main(void)
{
    if (mysig(SIGUSR1, user_handler) == SIG_ERR)
    {
        fprintf(stderr, "Ошибка установки обработчика SIGUSR1\n");
        exit(EXIT_FAILURE);
    }

    if (mysig(SIGUSR2, user_handler) == SIG_ERR)
    {
        fprintf(stderr, "Ошибка установки обработчика SIGUSR2\n");
        exit(EXIT_FAILURE);
    }

    printf("Процесс %d ожидает сигналов (SIGUSR1/SIGUSR2)...\n", getpid());

    while (1)
    {
        pause();
    }

    return 0;
}
