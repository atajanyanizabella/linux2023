#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 100

typedef struct msg_buffer
{
    long msg_type;
    char msg_text[MSG_SIZE];
} message_t;

void ping_signaler(int msqid_ping, int msqid_pong)
{
    message_t msg;
    while (1) {
        strcpy(msg.msg_text, "PING");
        msg.msg_type = 1;
        if (msgsnd(msqid_ping, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("Unable to send message\n");
            return;
        }
        if (msgrcv(msqid_pong, &msg, sizeof(msg.msg_text), 2, 0) == -1) {
            perror("Unable to receive message\n");
            return;
        }
        printf("Received: %s\n", msg.msg_text);
        sleep(1);
    }
}

int main()
{
    key_t key_ping;
    key_t key_pong;
    int msqid_ping;
    int msqid_pong;
    key_ping = ftok("progfile", 65);
    if (key_ping == -1) {
        perror("PING: ftok error\n");
        return 1;
    }
    int ping_id = msgget(key_ping, 0666 | IPC_CREAT);
    if (ping_id == -1) {
        perror("PING: Unable to get message queue\n");
        return 1;
    }
    key_pong = ftok("progfile", 65);
    if (key_pong == -1) {
        perror("PONG: ftok error\n");
        return 1;
    }
    int pong_id = msgget(key_pong, 0666 | IPC_CREAT);
    if (pong_id == -1) {
        perror("PONG: Unable to get message queue\n");
        return 1;
    }
    ping_signaler(msqid_ping, msqid_pong);
    return 0;
}
