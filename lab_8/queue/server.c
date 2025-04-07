#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

int msgid;

void sig_hndlr(int sig)
{
    printf("\nServer: Caught SIGINT. Cleaning up...\n");
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}

int main(void)
{
    Message msg_rcv, msg_snd;
    key_t key;

    signal(SIGINT, sig_hndlr);

    if ((key = ftok("../test.txt", 'A')) < 0)
    {
        perror("Server: can't receive key");
        exit(1);
    }

    if ((msgid = msgget(key, PERM | IPC_CREAT)) < 0)
    {
        perror("Server: can't create msg queue");
        exit(1);
    }

    printf("Server: Message queue created. Waiting for messages...\n");

    while (1)
    {
        if (msgrcv(msgid, &msg_rcv, MSGSZ, 1, 0) < 0)
        {
            perror("Server: msgrcv failed");
            kill(getpid(), SIGINT);  // аварийное завершение
        }

        printf("Server received: %s\n", msg_rcv.mtext);

        msg_snd.mtype = 2;
        snprintf(msg_snd.mtext, MSGSZ, "Hello from server, received: %s", msg_rcv.mtext);

        if (msgsnd(msgid, &msg_snd, MSGSZ, 0) < 0)
        {
            perror("Server: msgsnd failed");
            kill(getpid(), SIGINT);
        }
    }
}