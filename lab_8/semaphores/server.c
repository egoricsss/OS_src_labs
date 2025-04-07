#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.h"

Message* msgptr;
int shmid, semid;

void hndlr(int sig)
{
    signal(SIGINT, hndlr);

    // Отключение от области разделяемой памяти
    if (shmdt(msgptr) < 0) perror("Server: shmdt");

    // Удаление области разделяемой памяти
    if (shmctl(shmid, IPC_RMID, 0) < 0) perror("Server: shmctl");

    // Удаление семафоров
    if (semctl(semid, 0, IPC_RMID) < 0) perror("Server: semctl");

    printf("Server: cleaned up and exiting\n");
    exit(0);
}

int main()
{
    key_t key;
    signal(SIGINT, hndlr);

    if ((key = ftok("../test.txt", 'A')) < 0)
    {
        perror("Server: ftok");
        exit(1);
    }

    if ((shmid = shmget(key, sizeof(Message), PERM | IPC_CREAT)) < 0)
    {
        perror("Server: shmget");
        exit(1);
    }

    if ((msgptr = (Message*)shmat(shmid, NULL, 0)) == (Message*)-1)
    {
        perror("Server: shmat");
        exit(1);
    }

    if ((semid = semget(key, 2, PERM | IPC_CREAT)) < 0)
    {
        perror("Server: semget");
        exit(1);
    }

    printf("Server: ready and waiting for clients...\n");

    while (1)
    {
        // Ждём клиента
        semop(semid, proc_wait, 1);

        // Ждём разблокировки памяти
        semop(semid, mem_lock, 2);

        // Чтение сообщения
        printf("Server: received from client:\n%s", msgptr->buff);

        // Ответ
        snprintf(msgptr->buff, MAXBUFF, "Server response (PID %d)\n", getpid());

        // Освобождение ресурса
        semop(semid, mem_unlock, 1);
    }

    return 0;
}
