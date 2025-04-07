#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

int main(void)
{
    Message msg_snd, msg_rcv;
    key_t key;
    int msgid;

    if ((key = ftok("../test.txt", 'A')) < 0)
    {
        perror("Client: can't receive key");
        exit(1);
    }

    if ((msgid = msgget(key, PERM)) < 0)
    {
        perror("Client: can't access msg queue");
        exit(1);
    }

    msg_snd.mtype = 1;
    snprintf(msg_snd.mtext, MSGSZ, "Message from client PID %d", getpid());

    if (msgsnd(msgid, &msg_snd, MSGSZ, 0) < 0)
    {
        perror("Client: msgsnd failed");
        exit(1);
    }

    if (msgrcv(msgid, &msg_rcv, MSGSZ, 2, 0) < 0)
    {
        perror("Client: msgrcv failed");
        exit(1);
    }

    printf("Client received: %s\n", msg_rcv.mtext);
    return 0;
}