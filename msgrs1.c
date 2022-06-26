#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st
{
    long int my_msg_type;
    char some_text[BUFSIZ];
};

int main()
{
    int running = 1;
    int msgid, msgid2;
    struct my_msg_st some_data, send_data;
    long int msg_to_receive = 0;
    char buffer[BUFSIZ];

    send_data.my_msg_type = 1;
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    msgid2 = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (msgid == -1 || msgid2 == -1)
    {
        fprintf(stderr, "Massage get failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        printf("\nWriting...\n");
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_receive, 0) == -1)
        {

            fprintf(stderr, "Massage received failed with error:%d\n", errno);
            exit(EXIT_FAILURE);
        }

        printf("\nMd Ariful: %s", some_data.some_text);
        if (strncmp(some_data.some_text, "end", 3) == 0)
        {
            running = 0;
        }
        else
        {
            printf("You : ");
            fgets(buffer, BUFSIZ, stdin);
            send_data.my_msg_type = 1;
            strcpy(send_data.some_text, buffer);
            if (msgsnd(msgid2, (void *)&send_data, MAX_TEXT, 0) == -1)
            {
                fprintf(stderr, "Massage send failed\n");
                exit(EXIT_FAILURE);
            }
            if (strncmp(buffer, "end", 3) == 0)
            {
                running = 0;
            }
        }
    }
}