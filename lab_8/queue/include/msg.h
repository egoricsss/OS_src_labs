#ifndef MSG_H
#define MSG_H

#define MSGSZ 128
#define PERM 0666

typedef struct
{
    long mtype;         // тип сообщения
    char mtext[MSGSZ];  // текст сообщения
} Message;

#endif