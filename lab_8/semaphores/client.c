#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.h"

int main()
{
    key_t key;
    int shmid, semid;
    Message* msgptr;

    if ((key = ftok("../test.txt", 'A')) < 0)
    {
        perror("Client: ftok");
        exit(1);
    }

    if ((shmid = shmget(key, sizeof(Message), 0)) < 0)
    {
        perror("Client: shmget");
        exit(1);
    }

    if ((msgptr = (Message*)shmat(shmid, NULL, 0)) == (Message*)-1)
    {
        perror("Client: shmat");
        exit(1);
    }

    if ((semid = semget(key, 2, PERM)) < 0)
    {
        perror("Client: semget");
        exit(1);
    }

    // Блокировка памяти
    semop(semid, mem_lock, 2);

    // Уведомление сервера
    semop(semid, proc_start, 1);

    // Запись сообщения
    snprintf(msgptr->buff, MAXBUFF, "Hello from client (PID %d)\n", getpid());

    // Освобождение памяти
    semop(semid, mem_unlock, 1);

    // Ожидание ответа от сервера
    semop(semid, mem_lock, 2);

    printf("Client: received from server:\n%s", msgptr->buff);

    // Освобождение
    semop(semid, mem_unlock, 1);

    // Отсоединение
    if (shmdt(msgptr) < 0) perror("Client: shmdt");

    return 0;
}
