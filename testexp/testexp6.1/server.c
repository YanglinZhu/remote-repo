#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_KEY 1234
#define MAX_MSG_SIZE 1024
#define FILE_REQUEST 1
#define FILE_DATA 2

struct message {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

void send_file(int msgid, const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    struct message msg;
    msg.msg_type = FILE_DATA;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, msg.msg_text, MAX_MSG_SIZE)) > 0) {
        if (msgsnd(msgid, &msg, bytes_read, 0) == -1) {
            perror("msgsnd");
            close(fd);
            return;
        }
    }

    close(fd);
    // Send a message indicating end of file
    msg.msg_text[0] = '\0';
    msgsnd(msgid, &msg, 1, 0);
}

int main() {
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct message msg;

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), FILE_REQUEST, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Received request for file: %s\n", msg.msg_text);
        send_file(msgid, msg.msg_text);
    }

    return 0;
}
